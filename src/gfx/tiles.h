#pragma once
#include "game/level/level.h"
#include "core/vec2.h"

#define MAX_TILES	(LEVEL_CHUNK_SIZE * 4)

typedef struct tile_vertex
{
	vec2i8 position;
	vec2u16 texture_coords;
	int8_t z;
} tile_vertex;

typedef struct tile_quad
{
	tile_vertex vertices[4];
} tile_quad;

void tiles_init(void);
void tiles_terminate(void);

void set_tile_origin(vec2 position);

void set_tile_quad(tile_quad quad, uint16_t tile_index);
void draw_tiles(void);