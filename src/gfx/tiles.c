#include "tiles.h"

#include "camera.h"
#include "gfx.h"
#include "shader.h"
#include "texture.h"
#include "core/logging.h"
#include "core/mat3.h"
#include "game/level/level.h"
#include "glad/glad.h"
#include <stdlib.h>

static uint32_t s_vao;
static uint32_t s_vbo;
static uint32_t s_ibo;
static shader_h s_shader;

static vec2 s_origin;

static bool s_is_marked_dirty = false;
static tile_quad s_quads[MAX_TILES];

void tiles_init(void)
{
	LOG_MESSAGE("Initializing tiles...");

	glGenVertexArrays(1, &s_vao);
	glBindVertexArray(s_vao);

	glGenBuffers(1, &s_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_quads), s_quads, GL_DYNAMIC_DRAW);

	//x, y
	glVertexAttribPointer(0, 2, GL_BYTE, false, sizeof(tile_vertex), (void *)offsetof(tile_vertex, position));
	glEnableVertexAttribArray(0);

	//texture_x, texture_y
	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, true, sizeof(tile_vertex), (void *)offsetof(tile_vertex, texture_coords));
	glEnableVertexAttribArray(1);

	//z
	glVertexAttribPointer(2, 1, GL_BYTE, true, sizeof(tile_vertex), (void *)offsetof(tile_vertex, z));
	glEnableVertexAttribArray(2);

	uint16_t element_count;
	uint16_t *indices = gfx_generate_quad_indices(MAX_TILES, &element_count);
	glGenBuffers(1, &s_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_count * 2, indices, GL_STATIC_DRAW);
	free(indices);

	glBindVertexArray(0);

	s_shader = shader_load("res/shaders/tile");
}

void tiles_terminate(void)
{
	LOG_MESSAGE("Terminating tiles...");

	glDeleteVertexArrays(1, &s_vao);
	glDeleteBuffers(1, &s_vbo);
	glDeleteBuffers(1, &s_ibo);
}

void set_tile_origin(const vec2 position)
{
	s_origin = vec2_sub(position, (vec2){8, 8});
}

void set_tile_quad(const tile_quad quad, const uint16_t tile_index)
{
	s_quads[tile_index] = quad;
	s_is_marked_dirty = true;
}

void draw_tiles(void)
{
	shader_set(s_shader);

	if (s_is_marked_dirty)
	{
		glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(s_quads), s_quads);
	}

	texture_set(texture_get_id(tile_atlas_get_texture(level_get_tile_atlas())), 0);
	shader_set_int32_t(s_shader, "atlas_texture", 0);

	shader_set_vec2(s_shader, "origin", s_origin);
	shader_set_mat3(s_shader, "world_to_screen_matrix", world_to_screen_matrix(get_main_camera()));

	glBindVertexArray(s_vao);
	glDrawElements(GL_TRIANGLES, MAX_TILES * 6 / 4, GL_UNSIGNED_SHORT, 0);

	s_is_marked_dirty = false;
}