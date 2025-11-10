#pragma once

#include "object.h"
#include "tile.h"
#include "tile_atlas.h"

#define LEVEL_CHUNK_WIDTH	32
#define LEVEL_CHUNK_HEIGHT	16
#define LEVEL_CHUNK_SIZE	(LEVEL_CHUNK_WIDTH * LEVEL_CHUNK_HEIGHT)

#define TILE_SIZE_IN_PIXELS				16
#define LEVEL_CHUNK_WIDTH_IN_PIXELS		(TILE_SIZE_IN_PIXELS * LEVEL_CHUNK_WIDTH)
#define LEVEL_CHUNK_HEIGHT_IN_PIXELS	(TILE_SIZE_IN_PIXELS * LEVEL_CHUNK_HEIGHT)

#define OBJECTS_PER_CHUNK	64

typedef int16_t chunk_index;

typedef struct chunk_position
{
	int8_t x;
	int8_t y;
} chunk_position;

typedef struct tile_chunk
{
	tile tiles[LEVEL_CHUNK_SIZE];
} tile_chunk;

typedef struct object_chunk
{
	object object_data[OBJECTS_PER_CHUNK];
} object_chunk;

typedef struct level
{
	tile_atlas_h atlas;
	chunk_index chunk_count;
	chunk_position *position_data;
	tile_chunk *tile_data;
	object_chunk *object_data;
} level;

void levels_init();

void level_load(const char* filename);
void level_unload();

tile_atlas_h level_get_tile_atlas();
chunk_index level_get_chunk_count(void);
const chunk_position *level_get_chunk_position(const chunk_index index);
const tile_chunk *level_get_tile_chunk(const chunk_index index);
const object_chunk *level_get_object_chunk(const chunk_index index);

vec2 level_chunk_position_to_vec2(const chunk_position pos);