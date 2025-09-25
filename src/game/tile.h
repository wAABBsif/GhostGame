#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "gfx/texture.h"

typedef struct texture texture;

#define TILE_SIZE 16
#define TILE_CHUNK_SIZE 32
#define TILE_CHUNK_COMBINED_SIZE TILE_SIZE * TILE_CHUNK_SIZE
#define MAX_TILE_COUNT 1024
#define MAX_CHUNK_COUNT MAX_TILE_COUNT / TILE_CHUNK_SIZE

//LAYOUT: FFRRZZZZ IIIIIIII
typedef struct tile
{
	char transform;
	char textureIndex;
} tile;

typedef struct tile_chunk
{
	tile tiles[TILE_CHUNK_SIZE * TILE_CHUNK_SIZE];
} tile_chunk;

typedef struct tile_chunk_pos
{
	uint8_t x;
	uint8_t y;
} tile_chunk_pos;

#define TILE_FLIP_X_MASK     0b10000000
#define TILE_FLIP_Y_MASK     0b01000000
#define TILE_ROTATE_CCW_MASK 0b00100000
#define TILE_ROTATE_CW_MASK  0b00010000
#define TILE_Z_MASK          0b00001111

void tile_map_init();
void tile_map_terminate();
void tile_map_update();

bool tile_is_flip_x(tile t);
bool tile_is_flip_y(tile t);
bool tile_is_rotate_ccw(tile t);
bool tile_is_rotate_cw(tile t);
uint8_t tile_get_z(tile t);

void set_tilemap_atlas(texture_h t);