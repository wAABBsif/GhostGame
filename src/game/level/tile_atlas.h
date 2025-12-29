#pragma once
#include "core/vec2.h"
#include "game/ecs/components/component_collider.h"
#include "gfx/texture.h"

#define MAX_TILE_ATLASES	1

typedef struct tile_atlas tile_atlas;

typedef struct atlas_tile
{
	vec2u16 position;
	vec2u8 size;
	bool has_collision;
} atlas_tile;

typedef hash tile_atlas_h;

void tile_atlas_init(void);
void tile_atlas_clear(void);

tile_atlas_h tile_atlas_load(const char* filename);
void tile_atlas_unload(texture_h h);
tile_atlas_h tile_atlas_get(const char* name);

uint32_t tile_atlas_get_size(uint16_t tile_count);

atlas_tile tile_atlas_get_tile(tile_atlas_h atlas, uint16_t tile);
texture_h tile_atlas_get_texture(tile_atlas_h atlas);
uint16_t tile_atlas_get_tile_count(tile_atlas_h atlas);