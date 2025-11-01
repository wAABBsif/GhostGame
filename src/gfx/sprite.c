#include "sprite.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "camera.h"
#include "gfx.h"
#include "shader.h"
#include "texture.h"
#include "core/logging.h"
#include "core/mat3.h"
#include "glad/glad.h"

static uint32_t s_vao;
static uint32_t s_vbo;
static uint32_t s_ibo;
static shader_h s_shader;

static uint8_t s_texture_count;
static texture_h s_textures[MAX_SPRITE_TEXTURES];

static sprite_quad s_quads[MAX_SPRITES];
static uint16_t s_unsorted_count;
static uint16_t s_sorted_count;

void sprite_init(void)
{
	log_message("Initializing sprites...");

	glGenVertexArrays(1, &s_vao);
	glBindVertexArray(s_vao);

	glGenBuffers(1, &s_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sprite_quad) * MAX_SPRITES, s_quads, GL_DYNAMIC_DRAW);

	//x, y
	glVertexAttribPointer(0, 2, GL_SHORT, false, sizeof(sprite_vertex), (void *)offsetof(sprite_vertex, x));
	glEnableVertexAttribArray(0);

	//texture_x, texture_y
	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, true, sizeof(sprite_vertex), (void *)offsetof(sprite_vertex, texture_x));
	glEnableVertexAttribArray(1);

	//color
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, true, sizeof(sprite_vertex), (void *)offsetof(sprite_vertex, color));
	glEnableVertexAttribArray(2);

	//z
	glVertexAttribPointer(3, 1, GL_BYTE, true, sizeof(sprite_vertex), (void *)offsetof(sprite_vertex, z));
	glEnableVertexAttribArray(3);

	//use_camera_to_screen_matrix
	glVertexAttribIPointer(4, 1, GL_BYTE, sizeof(sprite_vertex), (void *)offsetof(sprite_vertex, use_camera_to_screen_matrix));
	glEnableVertexAttribArray(4);

	//texture_index
	glVertexAttribPointer(5, 1, GL_BYTE, false, sizeof(sprite_vertex), (void *)offsetof(sprite_vertex, texture_index));
	glEnableVertexAttribArray(5);

	const uint16_t quad_indices[6] = QUAD_INDICES;
	uint16_t indices[6 * MAX_SPRITES];
	for (int i = 0; i < 6 * MAX_SPRITES; i++)
	{
		const int index = i % 6;
		const int quad = i / 6;
		indices[i] = quad_indices[index] + 4 * quad;
	}
	glGenBuffers(1, &s_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	s_shader = shader_get("res/shaders/sprite");
}

void sprite_terminate(void)
{
	log_message("Terminating sprites...");

	glDeleteVertexArrays(1, &s_vao);
	glDeleteBuffers(1, &s_vbo);
	glDeleteBuffers(1, &s_ibo);

	s_texture_count = 0;
	s_sorted_count = 0;
	s_unsorted_count = 0;
}

void add_sprite_quad(const sprite_quad *sprite_quad, const bool is_sorted)
{
	if (!is_sorted)
	{
		s_quads[s_unsorted_count] = *sprite_quad;
		s_unsorted_count++;
	}
}

uint8_t sprite_get_texture_num(const texture_h h)
{
	for (uint8_t i = 0; i < s_texture_count; i++)
	{
		if (s_textures[i] == h)
			return i;
	}

	assert(s_texture_count < MAX_SPRITE_TEXTURES);

	s_textures[s_texture_count] = h;
	s_texture_count++;
	return s_texture_count - 1;
}

void draw_sprites(void)
{
	memmove(&s_quads[s_unsorted_count], &s_quads[MAX_SPRITES - s_sorted_count], sizeof(sprite_quad) * s_sorted_count);

	shader_set(s_shader);

	glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sprite_quad) * (s_unsorted_count + s_sorted_count), s_quads);

	for (int i = 0; i < s_texture_count; i++)
	{
		texture_set(s_textures[i], i);
		static char s_loc[16];

		sprintf(s_loc, "textures[%i]", i);
		shader_set_int32_t(s_shader, s_loc, i);
	}

	shader_set_mat3(s_shader, "world_to_screen_matrix", world_to_screen_matrix(get_main_camera()));
	shader_set_mat3(s_shader, "camera_to_screen_matrix", camera_to_screen_matrix(get_main_camera()));

	glBindVertexArray(s_vao);
	glDrawElements(GL_TRIANGLES, (s_unsorted_count + s_sorted_count) * 6, GL_UNSIGNED_SHORT, 0);

	s_texture_count = 0;
	s_unsorted_count = 0;
	s_sorted_count = 0;
}