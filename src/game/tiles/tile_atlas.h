#pragma once
#include "gfx/texture.h"

#define MAX_TILE_ATLASES	1

typedef struct tile_atlas tile_atlas;

typedef struct atlas_tile
{
	uint16_t texture_x;
	uint16_t texture_y;
	uint16_t texture_w;
	uint16_t texture_h;
} atlas_tile;

typedef hash tile_atlas_h;

void tile_atlas_init(void);
void tile_atlas_clear(void);

texture_h tile_atlas_load(const char* name);
void tile_atlas_unload(texture_h h);
texture_h tile_atlas_get(const char* name);

atlas_tile tile_atlas_get_tile(tile_atlas_h atlas, uint16_t tile);
texture_h tile_atlas_get_texture(tile_atlas_h atlas);
uint16_t tile_atlas_get_tile_count(tile_atlas_h atlas);