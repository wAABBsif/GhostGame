#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct texture texture;

//LAYOUT: FFRRZZZZ IIIIIIII
typedef struct tile
{
	char transform;
	char textureIndex;
} tile;

#define TILE_FLIP_X_MASK     0b10000000
#define TILE_FLIP_Y_MASK     0b01000000
#define TILE_ROTATE_CCW_MASK 0b00100000
#define TILE_ROTATE_CW_MASK  0b00010000
#define TILE_Z_MASK          0b00001111

bool tile_is_flip_x(tile t);
bool tile_is_flip_y(tile t);
bool tile_is_rotate_ccw(tile t);
bool tile_is_rotate_cw(tile t);
uint8_t tile_get_z(tile t);

void set_tilemap_atlas(texture t);