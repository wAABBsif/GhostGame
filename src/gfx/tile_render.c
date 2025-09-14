#include "tile_render.h"

#include "camera.h"
#include "gfx.h"
#include "shader.h"
#include "texture.h"
#include "core/mat3.h"
#include "glad/glad.h"

#define MAX_TILES 512

#define TILE_VERTEX_X_OFFSET 22
#define TILE_VERTEX_Y_OFFSET 12
#define TILE_VERTEX_Z_OFFSET 8
#define TILE_VERTEX_U_OFFSET 4
#define TILE_VERTEX_V_OFFSET 0

#define TILE_VERTEX_X_MASK 0b1111111111
#define TILE_VERTEX_Y_MASK 0b1111111111
#define TILE_VERTEX_Z_MASK 0b1111
#define TILE_VERTEX_U_MASK 0b1111
#define TILE_VERTEX_V_MASK 0b1111

static uint32_t s_vertex_array;
static uint32_t s_vertex_buffer;
static uint32_t s_index_buffer;
static shader s_shader;
static texture s_texture;

static tile_quad s_tile_quads[MAX_TILES];
static int s_tile_count;

void tile_rendering_init()
{
	glGenVertexArrays(1, &s_vertex_array);
	glBindVertexArray(s_vertex_array);

	glGenBuffers(1, &s_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_tile_quads), s_tile_quads, GL_DYNAMIC_DRAW);

	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(tile_vertex), 0);
	glEnableVertexAttribArray(0);

	const uint32_t quad_indices[6] = QUAD_INDICES;
	uint32_t indices[6 * MAX_TILES];
	for (int i = 0; i < 6 * MAX_TILES; i++)
	{
		const int index = i % 6;
		const int tile = i / 6;
		indices[i] = quad_indices[index] + 4 * tile;
	}
	glGenBuffers(1, &s_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	s_shader = shader_get("res/shaders/tile");
}

void tile_rendering_terminate()
{
	glDeleteVertexArrays(1, &s_vertex_array);
	glDeleteBuffers(1, &s_vertex_buffer);
	glDeleteBuffers(1, &s_index_buffer);
}

void tile_rendering_add_tile(const tile_quad tile)
{
	s_tile_quads[s_tile_count] = tile;
	s_tile_count++;
}

void tile_rendering_set_texture(const texture texture)
{
	s_texture = texture;
}

tile_vertex tile_vertex_set_x(const tile_vertex vertex, const uint32_t x)
{
	return vertex | (x << TILE_VERTEX_X_OFFSET);
}

tile_vertex tile_vertex_set_y(const tile_vertex vertex, const uint32_t y)
{
	return vertex | (y << TILE_VERTEX_Y_OFFSET);
}

tile_vertex tile_vertex_set_z(const tile_vertex vertex, const uint32_t index)
{
	return vertex | (index << TILE_VERTEX_Z_OFFSET);
}

tile_vertex tile_vertex_set_u(const tile_vertex vertex, const uint32_t u)
{
	return vertex | (u << TILE_VERTEX_U_OFFSET);
}

tile_vertex tile_vertex_set_v(const tile_vertex vertex, const uint32_t v)
{
	return vertex | (v << TILE_VERTEX_V_OFFSET);
}

uint16_t tile_vertex_get_x(const tile_vertex vertex)
{
	return (vertex >> TILE_VERTEX_X_OFFSET) & TILE_VERTEX_X_MASK;
}

uint16_t tile_vertex_get_y(const tile_vertex vertex)
{
	return (vertex >> TILE_VERTEX_Y_OFFSET) & TILE_VERTEX_Y_MASK;
}

uint8_t tile_vertex_get_z(const tile_vertex vertex)
{
	return (vertex >> TILE_VERTEX_Z_OFFSET) & TILE_VERTEX_Z_MASK;
}

uint8_t tile_vertex_get_u(const tile_vertex vertex)
{
	return (vertex >> TILE_VERTEX_U_OFFSET) & TILE_VERTEX_U_MASK;
}

uint8_t tile_vertex_get_v(const tile_vertex vertex)
{
	return (vertex >> TILE_VERTEX_V_OFFSET) & TILE_VERTEX_V_MASK;
}

mat3 m;

void tile_rendering_draw(void)
{
	tile_rendering_set_texture(texture_get("res/tiles/test_tile.png"));

	for (int i = 0; i < 4; i++)
	{
		tile_quad quad = {};

		quad.vertices[0] = tile_vertex_set_x(quad.vertices[0], i);
		quad.vertices[0] = tile_vertex_set_y(quad.vertices[0], 1);
		quad.vertices[0] = tile_vertex_set_z(quad.vertices[0], 0);
		quad.vertices[0] = tile_vertex_set_u(quad.vertices[0], i % 2 + 0);
		quad.vertices[0] = tile_vertex_set_v(quad.vertices[0], i / 2 + 1);

		quad.vertices[1] = tile_vertex_set_x(quad.vertices[1], i);
		quad.vertices[1] = tile_vertex_set_y(quad.vertices[1], 0);
		quad.vertices[1] = tile_vertex_set_z(quad.vertices[1], 0);
		quad.vertices[1] = tile_vertex_set_u(quad.vertices[1], i % 2 + 0);
		quad.vertices[1] = tile_vertex_set_v(quad.vertices[1], i / 2 + 0);

		quad.vertices[2] = tile_vertex_set_x(quad.vertices[2], i + 1);
		quad.vertices[2] = tile_vertex_set_y(quad.vertices[2], 0);
		quad.vertices[2] = tile_vertex_set_z(quad.vertices[2], 0);
		quad.vertices[2] = tile_vertex_set_u(quad.vertices[2], i % 2 + 1);
		quad.vertices[2] = tile_vertex_set_v(quad.vertices[2], i / 2 + 0);

		quad.vertices[3] = tile_vertex_set_x(quad.vertices[3], i + 1);
		quad.vertices[3] = tile_vertex_set_y(quad.vertices[3], 1);
		quad.vertices[3] = tile_vertex_set_z(quad.vertices[3], 0);
		quad.vertices[3] = tile_vertex_set_u(quad.vertices[3], i % 2 + 1);
		quad.vertices[3] = tile_vertex_set_v(quad.vertices[3], i / 2 + 1);

		tile_rendering_add_tile(quad);
	}

	glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tile_quad) * s_tile_count, s_tile_quads);

	shader_set(s_shader);
	texture_set(s_texture, 0);
	shader_set_uint32_t(s_shader, "tile_atlas", s_texture.id);
	shader_set_mat3(s_shader, "world_to_screen_matrix", world_to_screen_matrix(get_main_camera()));

	glBindVertexArray(s_vertex_array);
	glDrawElements(GL_TRIANGLES, 6 * s_tile_count, GL_UNSIGNED_INT, 0);
	s_tile_count = 0;
}