#pragma once

#include "object.h"
#include "tile.h"
#include "tile_atlas.h"

#define LEVEL_CHUNK_WIDTH	32
#define LEVEL_CHUNK_HEIGHT	16
#define LEVEL_CHUNK_SIZE	LEVEL_CHUNK_WIDTH * LEVEL_CHUNK_HEIGHT

#define OBJECTS_PER_CHUNK	64

typedef uint16_t chunk_index;

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
	chunk_index tile_count;
	chunk_position *position_data;
	tile_chunk *tile_data;
	object_chunk *object_data;
} level;