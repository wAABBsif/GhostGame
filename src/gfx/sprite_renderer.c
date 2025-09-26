#include "sprite_renderer.h"

#include <math.h>
#include <stdio.h>

#include "camera.h"
#include "core/vec2.h"
#include "gfx.h"
#include "shader.h"
#include "tile_renderer.h"
#include "core/logging.h"
#include "core/mat3.h"
#include "glad/glad.h"

static uint32_t s_vertex_array;
static uint32_t s_vertex_buffer;
static uint32_t s_index_buffer;
static shader_h s_shader;

static uint8_t s_texture_count;
static texture_h s_textures[MAX_SPRITE_TEXTURES];

static uint16_t s_quad_count;
static sprite_quad s_quads[MAX_SPRITES];

void sprite_renderer_init()
{
	glGenVertexArrays(1, &s_vertex_array);
	glBindVertexArray(s_vertex_array);

	glGenBuffers(1, &s_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_quads), NULL, GL_DYNAMIC_DRAW);

	glVertexAttribIPointer(0, 3, GL_UNSIGNED_INT, sizeof(tile_vertex), 0);
	glEnableVertexAttribArray(0);

	const uint16_t quad_indices[6] = QUAD_INDICES;
	uint16_t indices[6 * MAX_SPRITES];
	for (int i = 0; i < 6 * MAX_SPRITES; i++)
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
	return i;
}

void sprite_renderer_queue_sprite(sprite s)
{
	if (s_quad_count >= MAX_SPRITES)
		return log_warning("Max sprite count reached!");

	const vec2 position = {s.x, s.y};
	const float rotation = {s.rotation / 65536.0 * M_PI * 2};
	const vec2 scale = {s.scale_x / 16.0f, s.scale_y / 16.0f};

	const vec2 vertex_coords[4] =
	{
		{-0.5f, +0.5f},
		{-0.5f, -0.5f},
		{+0.5f, -0.5f},
		{+0.5f, +0.5f}
	};

	const uint16_t tex_coords[8] =
	{
		s.texture_x, s.texture_y,
		s.texture_x, s.texture_y - s.texture_h,
		s.texture_x + s.texture_w, s.texture_y - s.texture_h,
		s.texture_x + s.texture_w, s.texture_y,
	};

	const mat3 matrix = mat3_from_trs(position, rotation, scale);
	const uint8_t texture = sprite_renderer_get_texture(s.texture);

	sprite_quad quad;
	for (uint8_t i = 0; i < 4; i++)
	{
		const vec2 vpos = vec2_transform(vertex_coords[i], matrix);
		quad.vertices[i] = (sprite_vertex)
		{
			vpos.x, vpos.y,
			tex_coords[i * 2], tex_coords[i * 2 + 1],
			s.color,
			s.z, texture
		};
	}

	s_quads[s_quad_count] = quad;
	s_quad_count++;
}

void sprite_renderer_draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);

	shader_set(s_shader);

	for (uint8_t i = 0; i < s_texture_count; i++)
	{
		texture_set(s_textures[i], i);
		char* loc = "textures[%i]";
		sprintf(loc, loc, i);
		shader_set_uint32_t(s_shader, loc, texture_get_id(s_textures[i]));
	}

	shader_set_mat3(s_shader, "world_to_screen_matrix", world_to_screen_matrix(get_main_camera()));

	glBindVertexArray(s_vertex_array);
	glDrawElements(GL_TRIANGLES, 6 * s_quad_count, GL_UNSIGNED_SHORT, 0);

	s_quad_count = 0;
	s_texture_count = 0;
}