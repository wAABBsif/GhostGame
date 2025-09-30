#pragma once
#include <stdint.h>
#include "gfx/color.h"
#include "gfx/texture.h"

#define MAX_SPRITE_LAYERS 16
#define MAX_SPRITES_PER_LAYER 32
#define MAX_SPRITES_TOTAL MAX_SPRITES_PER_LAYER * MAX_SPRITE_LAYERS
#define MAX_SPRITE_TEXTURES 16

typedef struct sprite
{
	texture_h texture;
	uint16_t x;
	uint16_t y;

	uint16_t rotation;
	uint8_t scale_x;
	uint8_t scale_y;

	uint16_t texture_x;
	uint16_t texture_y;

	uint16_t texture_w;
	uint16_t texture_h;

	color    color;
	uint8_t  z;
} sprite;

void sprite_draw(sprite s);