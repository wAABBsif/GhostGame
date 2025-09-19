#include "tile.h"

#include <stdlib.h>
#include <string.h>

#include "core/game_time.h"
#include "core/logging.h"
#include "gfx/camera.h"
#include "gfx/tile_render.h"
#include "SDL3/SDL_keyboard.h"

static tile_chunk_pos s_chunk_pos[TILE_CHUNK_SIZE * TILE_CHUNK_SIZE];
static tile_chunk* s_chunks;
static int16_t s_chunk_count;
static int16_t s_active_chunks[4];

void tile_map_init()
{
	s_chunks = calloc(16, sizeof(tile_chunk));
	s_chunk_count = 16;
	for (int i = 0; i < s_chunk_count; i++)
	{
		s_chunk_pos[i] = (tile_chunk_pos){i % 4, i / 4};
		for (int j = 0; j < TILE_CHUNK_SIZE * TILE_CHUNK_SIZE; j++)
		{
			s_chunks[i].tiles[j].textureIndex = i % 4 + 1;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		s_active_chunks[i] = -1;
	}
}

void tile_map_terminate()
{

}

void tile_map_update()
{
	const camera *cam = get_main_camera();

	for (int i = 0; i < 4; i++)
	{
		s_active_chunks[i] = -1;
	}

	for (int i = 0; i < s_chunk_count; i++)
	{
		const vec2 chunk_pos = (vec2){s_chunk_pos[i].x * TILE_CHUNK_COMBINED_SIZE, s_chunk_pos[i].y * TILE_CHUNK_COMBINED_SIZE};
		if (chunk_pos.x - TILE_CHUNK_COMBINED_SIZE * 0.5 < cam->position.x && chunk_pos.x + TILE_CHUNK_COMBINED_SIZE * 0.5 > cam->position.x)
		{
			if (chunk_pos.y - TILE_CHUNK_COMBINED_SIZE * 0.5 < cam->position.y && chunk_pos.y + TILE_CHUNK_COMBINED_SIZE * 0.5 > cam->position.y)
				s_active_chunks[0] = i;

			if (chunk_pos.y + TILE_CHUNK_COMBINED_SIZE * 0.5 < cam->position.y && chunk_pos.y + TILE_CHUNK_COMBINED_SIZE * 1.5 > cam->position.y)
				s_active_chunks[2] = i;
		}

		if (chunk_pos.x + TILE_CHUNK_COMBINED_SIZE * 0.5 < cam->position.x && chunk_pos.x + TILE_CHUNK_COMBINED_SIZE * 1.5 > cam->position.x)
		{
			if (chunk_pos.y - TILE_CHUNK_COMBINED_SIZE * 0.5 < cam->position.y && chunk_pos.y + TILE_CHUNK_COMBINED_SIZE * 0.5 > cam->position.y)
				s_active_chunks[1] = i;

			if (chunk_pos.y + TILE_CHUNK_COMBINED_SIZE * 0.5  < cam->position.y && chunk_pos.y + TILE_CHUNK_COMBINED_SIZE * 1.5 > cam->position.y)
				s_active_chunks[3] = i;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (s_active_chunks[i] < 0)
			continue;

		const tile_chunk *chunk = &s_chunks[s_active_chunks[i]];
		const tile_chunk_pos pos = s_chunk_pos[s_active_chunks[i]];

		for (int tile = 0; tile < TILE_CHUNK_SIZE * TILE_CHUNK_SIZE; tile++)
		{
			const uint16_t x = tile % TILE_CHUNK_SIZE + pos.x * TILE_CHUNK_SIZE;
			const uint16_t y = tile / TILE_CHUNK_SIZE + pos.y * TILE_CHUNK_SIZE;

			tile_rendering_add_tile(x, y, chunk->tiles[tile]);
		}
	}
}

bool tile_is_flip_x(const tile t)
{
	return t.transform & TILE_FLIP_X_MASK;
}

bool tile_is_flip_y(const tile t)
{
	return t.transform & TILE_FLIP_Y_MASK;
}

bool tile_is_rotate_ccw(const tile t)
{
	return t.transform & TILE_ROTATE_CCW_MASK;
}

bool tile_is_rotate_cw(const tile t)
{
	return t.transform & TILE_ROTATE_CW_MASK;
}

uint8_t tile_get_z(const tile t)
{
	return t.transform & TILE_Z_MASK;
}

void set_tilemap_atlas(const texture t)
{
	tile_rendering_set_texture(t);
}