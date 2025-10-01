#include "tile_renderer.h"

#include <stdlib.h>
#include <time.h>

#include "camera.h"
#include "gfx.h"
#include "shader.h"
#include "texture.h"
#include "core/game_time.h"
#include "core/logging.h"
#include "core/mat3.h"
#include "game/tile.h"
#include "glad/glad.h"
#include "SDL3/SDL_keyboard.h"

#define TILE_VERTEX_X_OFFSET  22
#define TILE_VERTEX_Y_OFFSET  12
#define TILE_VERTEX_Z_OFFSET  8
#define TILE_VERTEX_UV_OFFSET 0
#define TILE_VERTEX_U_OFFSET  TILE_VERTEX_UV_OFFSET
#define TILE_VERTEX_V_OFFSET  4

#define TILE_VERTEX_X_MASK  0b1111111111
#define TILE_VERTEX_Y_MASK  0b1111111111
#define TILE_VERTEX_Z_MASK  0b1111
#define TILE_VERTEX_U_MASK  0b1111
#define TILE_VERTEX_V_MASK  0b1111
#define TILE_VERTEX_UV_MASK 0b11111111

static texture_h s_texture;
static draw_command *s_command;

void tile_renderer_init()
{
	s_command = create_draw_command(0);
	s_command->command = DRAW_COMMAND_TILE;

	glGenVertexArrays(1, &s_command->vao);
	glBindVertexArray(s_command->vao);

	glGenBuffers(1, &s_command->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, s_command->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tile_quad) * MAX_TILE_COUNT, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(tile_vertex), 0);
	glEnableVertexAttribArray(0);

	const uint16_t quad_indices[6] = QUAD_INDICES;
	uint16_t indices[6 * MAX_TILE_COUNT];
	for (int i = 0; i < 6 * MAX_TILE_COUNT; i++)
	{
		const int index = i % 6;
		const int tile = i / 6;
		indices[i] = quad_indices[index] + 4 * tile;
	}
	glGenBuffers(1, &s_command->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_command->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	s_command->shader = shader_get("res/shaders/tile");

	glBindVertexArray(0);
}

void tile_renderer_terminate()
{
	glDeleteVertexArrays(1, &s_command->vao);
	glDeleteBuffers(1, &s_command->vbo);
	glDeleteBuffers(1, &s_command->ebo);
	release_draw_command(s_command);
	s_command = NULL;
}

void tile_renderer_set_texture(const texture_h texture)
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
	return (vertex & ~(TILE_VERTEX_Z_MASK << TILE_VERTEX_Z_OFFSET)) | (z << TILE_VERTEX_Z_OFFSET);
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

tile_vertex tile_vertex_set_all(const uint32_t x, const uint32_t y, const uint32_t z, const uint32_t uv)
{
	return tile_vertex_set_uv(tile_vertex_set_z(tile_vertex_set_xy(0, x, y), z), uv);
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

void tile_renderer_clear_tiles()
{
	s_command->element_count = 0;
}

void tile_renderer_add_tile_chunk(const tile_chunk_pos chunk_pos, const tile_chunk *chunk)
{
	tile_quad quads[TILE_CHUNK_SIZE * TILE_CHUNK_SIZE];

	if (s_command->element_count >= MAX_TILE_COUNT * 6)
	{
		log_warning("Attempting to render too many tiles!");
		return;
	}

	for (uint16_t i = 0; i < TILE_CHUNK_SIZE * TILE_CHUNK_SIZE; i++)
	{
		//textureIndex 0 is reserved for "empty" tiles
		if (chunk->tiles[i].textureIndex == 0)
			continue;

		const uint16_t x = chunk_pos.x * TILE_CHUNK_SIZE + i % TILE_CHUNK_SIZE;
		const uint16_t y = chunk_pos.y * TILE_CHUNK_SIZE + i / TILE_CHUNK_SIZE;

		//rendering
		const uint8_t z = tile_get_z(chunk->tiles[i]);

		quads[i].vertices[0] = tile_vertex_set_all(x + 0, y + 1, z, chunk->tiles[i].textureIndex + 0);
		quads[i].vertices[1] = tile_vertex_set_all(x + 0, y + 0, z, chunk->tiles[i].textureIndex + 16);
		quads[i].vertices[2] = tile_vertex_set_all(x + 1, y + 0, z, chunk->tiles[i].textureIndex + 17);
		quads[i].vertices[3] = tile_vertex_set_all(x + 1, y + 1, z, chunk->tiles[i].textureIndex + 1);

		if (tile_is_flip_x(chunk->tiles[i]))
		{
			tile_quad nquad;
			nquad.vertices[0] = tile_vertex_set_x(quads[i].vertices[0], tile_vertex_get_x(quads[i].vertices[3]));
			nquad.vertices[1] = tile_vertex_set_x(quads[i].vertices[1], tile_vertex_get_x(quads[i].vertices[2]));
			nquad.vertices[2] = tile_vertex_set_x(quads[i].vertices[2], tile_vertex_get_x(quads[i].vertices[1]));
			nquad.vertices[3] = tile_vertex_set_x(quads[i].vertices[3], tile_vertex_get_x(quads[i].vertices[0]));

			quads[i] = nquad;
		}

		if (tile_is_flip_y(chunk->tiles[i]))
		{
			tile_quad nquad;
			nquad.vertices[0] = tile_vertex_set_y(quads[i].vertices[0], tile_vertex_get_y(quads[i].vertices[1]));
			nquad.vertices[1] = tile_vertex_set_y(quads[i].vertices[1], tile_vertex_get_y(quads[i].vertices[0]));
			nquad.vertices[2] = tile_vertex_set_y(quads[i].vertices[2], tile_vertex_get_y(quads[i].vertices[3]));
			nquad.vertices[3] = tile_vertex_set_y(quads[i].vertices[3], tile_vertex_get_y(quads[i].vertices[2]));

			quads[i] = nquad;
		}

		if (tile_is_rotate_ccw(chunk->tiles[i]))
		{
			tile_quad nquad;
			nquad.vertices[0] = tile_vertex_set_xy(quads[i].vertices[0], tile_vertex_get_x(quads[i].vertices[3]), tile_vertex_get_y(quads[i].vertices[3]));
			nquad.vertices[1] = tile_vertex_set_xy(quads[i].vertices[1], tile_vertex_get_x(quads[i].vertices[0]), tile_vertex_get_y(quads[i].vertices[0]));
			nquad.vertices[2] = tile_vertex_set_xy(quads[i].vertices[2], tile_vertex_get_x(quads[i].vertices[1]), tile_vertex_get_y(quads[i].vertices[1]));
			nquad.vertices[3] = tile_vertex_set_xy(quads[i].vertices[3], tile_vertex_get_x(quads[i].vertices[2]), tile_vertex_get_y(quads[i].vertices[2]));

			quads[i] = nquad;
		}

		if (tile_is_rotate_cw(chunk->tiles[i]))
		{
			tile_quad nquad;
			nquad.vertices[0] = tile_vertex_set_xy(quads[i].vertices[0], tile_vertex_get_x(quads[i].vertices[1]), tile_vertex_get_y(quads[i].vertices[1]));
			nquad.vertices[1] = tile_vertex_set_xy(quads[i].vertices[1], tile_vertex_get_x(quads[i].vertices[2]), tile_vertex_get_y(quads[i].vertices[2]));
			nquad.vertices[2] = tile_vertex_set_xy(quads[i].vertices[2], tile_vertex_get_x(quads[i].vertices[3]), tile_vertex_get_y(quads[i].vertices[3]));
			nquad.vertices[3] = tile_vertex_set_xy(quads[i].vertices[3], tile_vertex_get_x(quads[i].vertices[0]), tile_vertex_get_y(quads[i].vertices[0]));

			quads[i] = nquad;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, s_command->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, s_command->element_count / 6 * sizeof(tile_quad), sizeof(tile_quad) * TILE_CHUNK_SIZE * TILE_CHUNK_SIZE, quads);
	s_command->element_count += TILE_CHUNK_SIZE * TILE_CHUNK_SIZE * 6;
}

void tile_renderer_draw()
{
	texture_set(s_texture, 0);
	shader_set_int32_t(s_command->shader, "tile_atlas", 0);
	shader_set_mat3(s_command->shader, "world_to_screen_matrix", world_to_screen_matrix(get_main_camera()));
}