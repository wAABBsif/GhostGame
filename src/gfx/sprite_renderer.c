#include "sprite_renderer.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "camera.h"
#include "core/vec2.h"
#include "shader.h"
#include "core/logging.h"
#include "core/mat3.h"
#include "glad/glad.h"

static uint8_t s_texture_count;
static texture_h s_textures[MAX_SPRITE_TEXTURES];

static sprite_quad s_quads_unsorted[MAX_SPRITES_UNSORTED];
static sprite_quad s_quads_sorted[MAX_SPRITES_SORTED];

static draw_command *s_command_unsorted;
static draw_command *s_command_sorted;

void sprite_renderer_init()
{
	s_command_unsorted = create_sprite_render_command(MAX_SPRITES_UNSORTED, 1, DRAW_COMMAND_SPRITE_UNSORTED);
	s_command_sorted = create_sprite_render_command(MAX_SPRITES_UNSORTED, 2, DRAW_COMMAND_SPRITE_SORTED);
}

void sprite_renderer_terminate()
{
	s_command_unsorted = destroy_sprite_render_command(s_command_unsorted);
	s_command_sorted = destroy_sprite_render_command(s_command_sorted);
}

uint8_t sprite_renderer_get_texture(const texture_h h)
{
	uint8_t i;
	for (i = 0; i < s_texture_count; i++)
	{
		if (s_textures[i] == h)
			return i;
	}

	if (i >= MAX_SPRITE_TEXTURES)
	{
		log_warning("Max sprite texture count reached!");
		return 0;
	}

	s_textures[i] = h;
	s_texture_count++;
	return i;
}

draw_command *create_sprite_render_command(const uint16_t sprite_limit, const uint16_t draw_order, const command_type type)
{
	draw_command *command = create_draw_command(draw_order);
	command->command = type;

	glGenVertexArrays(1, &command->vao);
	glBindVertexArray(command->vao);

	glGenBuffers(1, &command->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, command->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sprite_quad) * sprite_limit, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribIPointer(0, 3, GL_UNSIGNED_INT, sizeof(sprite_vertex), 0);
	glEnableVertexAttribArray(0);

	const uint16_t quad_indices[6] = QUAD_INDICES;
	uint16_t indices[6 * sprite_limit];
	for (int i = 0; i < 6 * sprite_limit; i++)
	{
		const int index = i % 6;
		const int tile = i / 6;
		indices[i] = quad_indices[index] + 4 * tile;
	}
	glGenBuffers(1, &command->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, command->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	command->shader = shader_get("res/shaders/sprite");
	return command;
}

draw_command *destroy_sprite_render_command(draw_command *cmd)
{
	glDeleteVertexArrays(1, &cmd->vao);
	glDeleteBuffers(1, &cmd->vbo);
	glDeleteBuffers(1, &cmd->ebo);
	release_draw_command(cmd);
	return NULL;
}

sprite_quad sprite_renderer_create_quad(sprite s)
{
	const vec2 position = {s.x, s.y};
	const float rotation = {s.rotation / 65536.0f * M_PI * 2};
	const vec2 scale = {s.scale_x, s.scale_y};

	const vec2 vertex_coords[4] =
	{
		{-0.5f, +0.5f},
		{-0.5f, -0.5f},
		{+0.5f, -0.5f},
		{+0.5f, +0.5f}
	};

	const uint16_t tex_coords[8] =
	{
		s.texture_x, s.texture_y - s.texture_h,
		s.texture_x, s.texture_y,
		s.texture_x + s.texture_w, s.texture_y,
		s.texture_x + s.texture_w, s.texture_y - s.texture_h,
	};

	const mat3 matrix = mat3_from_trs(position, rotation, scale);

	const uint8_t texture = sprite_renderer_get_texture(s.texture);
	int32_t texture_w, texture_h;
	texture_get_size(s.texture, &texture_w, &texture_h);

	sprite_quad quad;
	for (uint8_t i = 0; i < 4; i++)
	{
		const vec2 vpos = vec2_transform(vertex_coords[i], matrix);
		const vec2 vtex_coords =
		{
			(float)tex_coords[i * 2] / (float)texture_w * 65535.0f,
			(float)tex_coords[i * 2 + 1] / (float)texture_h * 65535
		};

		quad.vertices[i] = (sprite_vertex)
		{
			lroundf(vpos.x), lroundf(vpos.y),
			(uint16_t)vtex_coords.x, (uint16_t)vtex_coords.y,
			s.color,
			s.z, texture
		};
	}

	return quad;
}

void sprite_renderer_add_quad_unsorted(const sprite_quad quad)
{
	s_quads_unsorted[s_command_unsorted->element_count / 6] = quad;
	s_command_unsorted->element_count += 6;
}

void sprite_renderer_add_quad_sorted(const sprite_quad quad)
{
	s_quads_sorted[s_command_sorted->element_count / 6] = quad;
	s_command_sorted->element_count += 6;
}

void sprite_renderer_clear_frame()
{
	s_command_sorted->element_count = 0;
	s_command_unsorted->element_count = 0;
	s_texture_count = 0;
}

void sprite_renderer_draw(const draw_command *cmd, const sprite_quad *quads)
{
	glBindBuffer(GL_ARRAY_BUFFER, cmd->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sprite_quad) * cmd->element_count / 6, quads);

	for (int i = 0; i < s_texture_count; i++)
	{
		texture_set(s_textures[i], i);
		static char s_loc[16];

		sprintf(s_loc, "textures[%i]", i);
		shader_set_int32_t(cmd->shader, s_loc, i);
	}

	shader_set_mat3(cmd->shader, "world_to_screen_matrix", world_to_screen_matrix(get_main_camera()));
}

void sprite_renderer_draw_unsorted(draw_command *cmd)
{
	sprite_renderer_draw(cmd, s_quads_unsorted);
}

void sprite_renderer_draw_sorted(draw_command *cmd)
{
	static sprite_quad s_n_quads[MAX_SPRITES_SORTED];
	uint16_t quad_index = 0;

	for (int i = 0; i < MAX_SPRITE_LAYERS; i++)
	{
		for (int j = 0; j < cmd->element_count / 6; j++)
		{
			if (s_quads_sorted[j].vertices[0].z == i)
			{
				s_n_quads[quad_index] = s_quads_sorted[j];
				quad_index++;
			}
		}
	}

	sprite_renderer_draw(cmd, s_n_quads);
}