#include "tile_render.h"

#include "camera.h"
#include "gfx.h"
#include "shader.h"
#include "texture.h"
#include "core/game_time.h"
#include "core/logging.h"
#include "core/mat3.h"
#include "game/tile.h"
#include "glad/glad.h"

#define MAX_TILES 512

#define TILE_VERTEX_X_OFFSET  22
#define TILE_VERTEX_Y_OFFSET  12
#define TILE_VERTEX_Z_OFFSET  8
#define TILE_VERTEX_U_OFFSET  4
#define TILE_VERTEX_V_OFFSET  0
#define TILE_VERTEX_UV_OFFSET 0

#define TILE_VERTEX_X_MASK  0b1111111111
#define TILE_VERTEX_Y_MASK  0b1111111111
#define TILE_VERTEX_Z_MASK  0b1111
#define TILE_VERTEX_U_MASK  0b1111
#define TILE_VERTEX_V_MASK  0b1111
#define TILE_VERTEX_UV_MASK 0b11111111

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
	if (s_tile_count >= MAX_TILES)
	{
		log_error("Max tile count reached!");
		return;
	}

	s_tile_quads[s_tile_count] = tile;
	s_tile_count++;
}

void tile_rendering_set_texture(const texture texture)
{
	s_texture = texture;
}

tile_vertex tile_vertex_set_x(const tile_vertex vertex, const uint32_t x)
{
	return (vertex & ~(TILE_VERTEX_X_MASK << TILE_VERTEX_X_OFFSET)) | (x << TILE_VERTEX_X_OFFSET);
}

tile_vertex tile_vertex_set_y(const tile_vertex vertex, const uint32_t y)
{
	return (vertex & ~(TILE_VERTEX_Y_MASK << TILE_VERTEX_Y_OFFSET)) | (y << TILE_VERTEX_Y_OFFSET);
}

tile_vertex tile_vertex_set_z(const tile_vertex vertex, const uint32_t z)
{
	return (vertex & ~(TILE_VERTEX_Z_MASK << TILE_VERTEX_Z_OFFSET)) | (z << TILE_VERTEX_Y_OFFSET);
}

tile_vertex tile_vertex_set_u(const tile_vertex vertex, const uint32_t u)
{
	return (vertex & ~(TILE_VERTEX_U_MASK << TILE_VERTEX_U_OFFSET)) | (u << TILE_VERTEX_U_OFFSET);
}

tile_vertex tile_vertex_set_v(const tile_vertex vertex, const uint32_t v)
{
	return (vertex & ~(TILE_VERTEX_V_MASK << TILE_VERTEX_V_OFFSET)) | (v << TILE_VERTEX_V_OFFSET);
}

tile_vertex tile_vertex_set_xy(const tile_vertex vertex, const uint32_t x, const uint32_t y)
{
	return tile_vertex_set_y(tile_vertex_set_x(vertex, x), y);
}

tile_vertex tile_vertex_set_uv(const tile_vertex vertex, const uint32_t uv)
{
	return vertex | (uv << TILE_VERTEX_UV_OFFSET);
}

tile_vertex tile_vertex_set_all(const tile_vertex vertex, const uint32_t x, const uint32_t y, const uint32_t z, const uint32_t uv)
{
	return tile_vertex_set_uv(tile_vertex_set_z(tile_vertex_set_xy(vertex, x, y), z), uv);
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

uint8_t tile_vertex_get_uv(const tile_vertex vertex)
{
	return (vertex >> TILE_VERTEX_UV_OFFSET) & TILE_VERTEX_UV_MASK;
}

mat3 m;

void tile_rendering_draw(void)
{
	set_tilemap_atlas(texture_get("res/tiles/test_tile.png"));

	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			tile_draw(i, j, (tile){TILE_FLIP_X_MASK, 1});
		}
	}

	log_message("%f", game_time_get_fps());

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