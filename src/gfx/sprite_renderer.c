#include "sprite_renderer.h"

#include <math.h>
#include <stdio.h>

#include "camera.h"
#include "core/vec2.h"
#include "shader.h"
#include "core/logging.h"
#include "core/mat3.h"
#include "glad/glad.h"
#include "core/game_time.h"

static uint8_t s_texture_count;
static texture_h s_textures[MAX_SPRITE_TEXTURES];

static sprite_quad s_quads[MAX_SPRITES];

static draw_command *s_command;

void sprite_renderer_init()
{
	s_command = create_draw_command(1);
	s_command->command = DRAW_COMMAND_SPRITE;

	glGenVertexArrays(1, &s_command->vao);
	glBindVertexArray(s_command->vao);

	glGenBuffers(1, &s_command->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, s_command->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_quads), NULL, GL_DYNAMIC_DRAW);

	glVertexAttribIPointer(0, 3, GL_UNSIGNED_INT, sizeof(sprite_vertex), 0);
	glEnableVertexAttribArray(0);

	const uint16_t quad_indices[6] = QUAD_INDICES;
	uint16_t indices[6 * MAX_SPRITES];
	for (int i = 0; i < 6 * MAX_SPRITES; i++)
	{
		const int index = i % 6;
		const int tile = i / 6;
		indices[i] = quad_indices[index] + 4 * tile;
	}
	glGenBuffers(1, &s_command->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_command->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	s_command->shader = shader_get("res/shaders/sprite");
}

void sprite_renderer_terminate()
{
	glDeleteVertexArrays(1, &s_command->vao);
	glDeleteBuffers(1, &s_command->vbo);
	glDeleteBuffers(1, &s_command->ebo);
	release_draw_command(s_command);
	s_command = NULL;
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

void sprite_renderer_add_sprite(const sprite s)
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
	uint32_t texture_w, texture_h;
	texture_get_size(s.texture, &texture_w, &texture_h);

	sprite_quad quad;
	for (uint8_t i = 0; i < 4; i++)
	{
		const vec2 vpos = vec2_transform(vertex_coords[i], matrix);
		quad.vertices[i] = (sprite_vertex)
		{
			vpos.x + 0.5f, vpos.y + 0.5f,
			tex_coords[i * 2] / (float)texture_w * 65535, tex_coords[i * 2 + 1] / (float)texture_h * 65535,
			s.color,
			s.z, texture
		};
	}


	s_quads[s_command->element_count / 6] = quad;
	s_command->element_count += 6;
}

void sprite_renderer_clear_frame()
{
	s_command->element_count = 0;
	s_texture_count = 0;
}

void sprite_renderer_draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, s_command->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sprite_quad) * s_command->element_count / 6, s_quads);

	for (int i = 0; i < s_texture_count; i++)
	{
		texture_set(s_textures[i], i);
		char loc[16];

		const char* format = "textures[%i]";
		sprintf(loc, format, i);
		shader_set_int32_t(s_command->shader, loc, i);
	}

	shader_set_mat3(s_command->shader, "world_to_screen_matrix", world_to_screen_matrix(get_main_camera()));
}