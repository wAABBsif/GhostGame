#include "tile.h"

#include <stdlib.h>
#include <string.h>

#include "core/game_time.h"
#include "core/logging.h"
#include "gfx/camera.h"
#include "gfx/tile_renderer.h"
#include "SDL3/SDL_keyboard.h"

static tile_chunk_pos s_chunk_pos[MAX_CHUNK_COUNT * MAX_CHUNK_COUNT];
static tile_chunk* s_chunks;
static int16_t s_chunk_count;
static int16_t s_prev_active_chunks[4];
static int16_t s_active_chunks[4];

void tile_map_init()
{
	s_chunks = calloc(4, sizeof(tile_chunk));
	s_chunk_count = 4;
	for (int i = 0; i < 4; i++)
	{
		s_chunk_pos[i] = (tile_chunk_pos){i % 2, i / 2};
	}

	for (int i = 0; i < TILE_CHUNK_SIZE * TILE_CHUNK_SIZE; i++)
	{
		if (i % TILE_CHUNK_SIZE > 10 && i % TILE_CHUNK_SIZE < 22 && i / TILE_CHUNK_SIZE > 10 && i / TILE_CHUNK_SIZE < 22)
			continue;

		s_chunks[0].tiles[i].transform = 0;
		s_chunks[0].tiles[i].textureIndex = 7;

		if (i % TILE_CHUNK_SIZE == 0)
		{
			s_chunks[0].tiles[i].transform = TILE_ROTATE_CW_MASK;
			s_chunks[0].tiles[i].textureIndex = 5;
		}

		if (i / TILE_CHUNK_SIZE == 0)
		{
			s_chunks[0].tiles[i].transform = TILE_FLIP_Y_MASK;
			s_chunks[0].tiles[i].textureIndex = 5;
		}

		if (i % TILE_CHUNK_SIZE == 31)
		{
			s_chunks[0].tiles[i].transform = TILE_ROTATE_CCW_MASK;
			s_chunks[0].tiles[i].textureIndex = 5;
		}

		if (i / TILE_CHUNK_SIZE == 31)
		{
			s_chunks[0].tiles[i].transform = 0;
			s_chunks[0].tiles[i].textureIndex = 5;
		}

		if (i % TILE_CHUNK_SIZE == 10 && i / TILE_CHUNK_SIZE > 10 && i / TILE_CHUNK_SIZE < 22)
		{
			s_chunks[0].tiles[i].transform = TILE_ROTATE_CCW_MASK;
			s_chunks[0].tiles[i].textureIndex = 5;
		}

		if (i / TILE_CHUNK_SIZE == 10 && i % TILE_CHUNK_SIZE > 10 && i % TILE_CHUNK_SIZE < 22)
		{
			s_chunks[0].tiles[i].transform = 0;
			s_chunks[0].tiles[i].textureIndex = 5;
		}

		if (i % TILE_CHUNK_SIZE == 22 && i / TILE_CHUNK_SIZE > 10 && i / TILE_CHUNK_SIZE < 22)
		{
			s_chunks[0].tiles[i].transform = TILE_ROTATE_CW_MASK;
			s_chunks[0].tiles[i].textureIndex = 5;
		}

		if (i / TILE_CHUNK_SIZE == 22 && i % TILE_CHUNK_SIZE > 10 && i % TILE_CHUNK_SIZE < 22)
		{
			s_chunks[0].tiles[i].transform = TILE_FLIP_Y_MASK;
			s_chunks[0].tiles[i].textureIndex = 5;
		}

		if (i == 10 * 32 + 10)
		{
			s_chunks[0].tiles[i].transform = 0;
			s_chunks[0].tiles[i].textureIndex = 6;
		}

		if (i == 10 * 32 + 22)
		{
			s_chunks[0].tiles[i].transform = TILE_FLIP_X_MASK;
			s_chunks[0].tiles[i].textureIndex = 6;
		}

		if (i == 22 * 32 + 10)
		{
			s_chunks[0].tiles[i].transform = TILE_FLIP_Y_MASK;
			s_chunks[0].tiles[i].textureIndex = 6;
		}

		if (i == 22 * 32 + 22)
		{
			s_chunks[0].tiles[i].transform = TILE_FLIP_X_MASK | TILE_FLIP_Y_MASK;
			s_chunks[0].tiles[i].textureIndex = 6;
		}

		if (i == 0 * 32 + 0)
		{
			s_chunks[0].tiles[i].transform = TILE_FLIP_Y_MASK | TILE_FLIP_X_MASK;
			s_chunks[0].tiles[i].textureIndex = 4;
		}

		if (i == 0 * 32 + 31)
		{
			s_chunks[0].tiles[i].transform = TILE_FLIP_Y_MASK;
			s_chunks[0].tiles[i].textureIndex = 4;
		}

		if (i == 31 * 32 + 0)
		{
			s_chunks[0].tiles[i].transform = TILE_FLIP_X_MASK;
			s_chunks[0].tiles[i].textureIndex = 4;
		}

		if (i == 31 * 32 + 31)
		{
			s_chunks[0].tiles[i].transform = 0;
			s_chunks[0].tiles[i].textureIndex = 4;
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

	for (int i = 0; i < s_chunk_count; i++)
	{
		const vec2 chunk_pos = (vec2){s_chunk_pos[i].x * TILE_CHUNK_COMBINED_SIZE, s_chunk_pos[i].y * TILE_CHUNK_COMBINED_SIZE};
		if (chunk_pos.x - TILE_CHUNK_COMBINED_SIZE * 0.5 <= cam->position.x && chunk_pos.x + TILE_CHUNK_COMBINED_SIZE * 0.5 >= cam->position.x)
		{
			if (chunk_pos.y - TILE_CHUNK_COMBINED_SIZE * 0.5 <= cam->position.y && chunk_pos.y + TILE_CHUNK_COMBINED_SIZE * 0.5 >= cam->position.y)
				s_active_chunks[0] = i;

			if (chunk_pos.y + TILE_CHUNK_COMBINED_SIZE * 0.5 <= cam->position.y && chunk_pos.y + TILE_CHUNK_COMBINED_SIZE * 1.5 >= cam->position.y)
				s_active_chunks[2] = i;
		}

		if (chunk_pos.x + TILE_CHUNK_COMBINED_SIZE * 0.5 <= cam->position.x && chunk_pos.x + TILE_CHUNK_COMBINED_SIZE * 1.5 >= cam->position.x)
		{
			if (chunk_pos.y - TILE_CHUNK_COMBINED_SIZE * 0.5 <= cam->position.y && chunk_pos.y + TILE_CHUNK_COMBINED_SIZE * 0.5 >= cam->position.y)
				s_active_chunks[1] = i;

			if (chunk_pos.y + TILE_CHUNK_COMBINED_SIZE * 0.5  <= cam->position.y && chunk_pos.y + TILE_CHUNK_COMBINED_SIZE * 1.5 >= cam->position.y)
				s_active_chunks[3] = i;
		}
	}

	if (memcmp(s_active_chunks, s_prev_active_chunks, sizeof(s_prev_active_chunks)) != 0)
	{
		tile_renderer_clear_tiles();

		for (int i = 0; i < 4; i++)
		{
			if (s_active_chunks[i] < 0)
				continue;

			const tile_chunk *chunk = &s_chunks[s_active_chunks[i]];
			const tile_chunk_pos pos = s_chunk_pos[s_active_chunks[i]];

			tile_renderer_add_tile_chunk(pos, chunk);
		}
	}

	memcpy(s_prev_active_chunks, s_active_chunks, sizeof(s_active_chunks));

	log_message("%i %i %i %i", s_active_chunks[0], s_active_chunks[1], s_active_chunks[2], s_active_chunks[3]);
	//
	set_tilemap_atlas(texture_get("res/tiles/test_tile.png"));
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

void set_tilemap_atlas(const texture_h t)
{
	tile_renderer_set_texture(t);
}