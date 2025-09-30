#include "sprite_renderer.h"

#include <math.h>
#include <stdio.h>

#include "camera.h"
#include "core/vec2.h"
#include "gfx.h"
#include "shader.h"
#include "core/game_time.h"
#include "core/logging.h"
#include "core/mat3.h"
#include "glad/glad.h"

static uint32_t s_vertex_array;
static uint32_t s_vertex_buffer;
static uint32_t s_index_buffer;
static shader_h s_shader;

static uint8_t s_texture_count;
static texture_h s_textures[MAX_SPRITE_TEXTURES];

static uint16_t s_quad_counts[MAX_SPRITE_LAYERS];
static sprite_quad s_quads[MAX_SPRITES_PER_LAYER][MAX_SPRITE_LAYERS];

void sprite_renderer_init()
{
	glGenVertexArrays(1, &s_vertex_array);
	glBindVertexArray(s_vertex_array);

	glGenBuffers(1, &s_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_quads), NULL, GL_DYNAMIC_DRAW);

	glVertexAttribIPointer(0, 3, GL_UNSIGNED_INT, sizeof(sprite_vertex), 0);
	glEnableVertexAttribArray(0);

	const uint16_t quad_indices[6] = QUAD_INDICES;
	uint16_t indices[6 * MAX_SPRITES_TOTAL];
	for (int i = 0; i < 6 * MAX_SPRITES_TOTAL; i++)
	{
		const int index = i % 6;
		const int tile = i / 6;
		indices[i] = quad_indices[index] + 4 * tile;
	}
	glGenBuffers(1, &s_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	s_shader = shader_get("res/shaders/sprite");
}

void sprite_renderer_terminate()
{
	glDeleteVertexArrays(1, &s_vertex_array);
	glDeleteBuffers(1, &s_vertex_buffer);
	glDeleteBuffers(1, &s_index_buffer);
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

void sprite_renderer_queue_sprite(sprite s)
{
	while (s_quad_counts[s.z] >= MAX_SPRITES_PER_LAYER)
	{
		if (s.z >= MAX_SPRITE_LAYERS)
			return log_error("Cannot render sprite!");
		s.z++;
	}

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


	s_quads[s.z][s_quad_counts[s.z]] = quad;
	s_quad_counts[s.z]++;
}

void sprite_renderer_draw()
{
	sprite s2;
	s2.x = 120 + cosf(game_time_get_elapsed()) * 60;
	s2.y = 24;
	s2.rotation = 0;
	s2.scale_x = 0x10;
	s2.scale_y = 0x10;
	s2.color = WHITE;
	s2.z = 3;
	s2.texture = texture_get("res/sprites/test2.png");
	s2.texture_w = 16;
	s2.texture_h = 16;
	s2.texture_x = 0;
	s2.texture_y = 16;
	sprite_draw(s2);

	sprite s;
	s.x = 120;
	s.y = 24;
	s.rotation = (uint16_t)(game_time_get_elapsed() * 5000);
	s.scale_x = 0x10;
	s.scale_y = 0x10;
	s.color = WHITE;
	s.z = 2;
	s.texture = texture_get("res/sprites/test.png");
	s.texture_w = 16;
	s.texture_h = 16;
	s.texture_x = 0;
	s.texture_y = 16;
	sprite_draw(s);

	glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);

	uint32_t sprite_count = 0;
	for (int i = 0; i < MAX_SPRITE_LAYERS; i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, sprite_count * sizeof(sprite_quad), sizeof(sprite_quad) * s_quad_counts[i], s_quads[i]);
		sprite_count += s_quad_counts[i];
	}

	shader_set(s_shader);

	for (int i = 0; i < s_texture_count; i++)
	{
		texture_set(s_textures[i], i);
		char loc[16];

		const char* format = "textures[%i]";
		sprintf(loc, format, i);
		shader_set_int32_t(s_shader, loc, i);
	}

	shader_set_mat3(s_shader, "world_to_screen_matrix", world_to_screen_matrix(get_main_camera()));

	glBindVertexArray(s_vertex_array);
	glDrawElements(GL_TRIANGLES, 6 * sprite_count, GL_UNSIGNED_SHORT, 0);

	for (int i = 0; i < MAX_SPRITE_LAYERS; i++)
		s_quad_counts[i] = 0;
	s_texture_count = 0;
}