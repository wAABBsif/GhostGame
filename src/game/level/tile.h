#pragma once
#include <stdint.h>

typedef struct tile
{
	uint16_t tile_index;
	int8_t z;
	uint8_t flip_x : 1;
	uint8_t flip_y : 1;
	uint8_t rotate_ccw : 1;
	uint8_t rotate_cw : 1;
	uint8_t additional_flags : 4;
} tile;
