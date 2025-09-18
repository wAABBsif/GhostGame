#include "tile.h"

#include <string.h>

#include "gfx/tile_render.h"

static tile_chunk_ref s_chunk_refs[TILE_CHUNK_SIZE * TILE_CHUNK_SIZE];
static tile_chunk* s_tile_chunks;
static uint16_t s_tile_chunk_count;

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