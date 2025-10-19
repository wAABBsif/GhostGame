#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "gfx/color.h"
#include "gfx/texture.h"

#define MAX_SPRITES_UNSORTED 256
#define MAX_SPRITES_SORTED 128
#define MAX_SPRITE_TEXTURES 16
#define MAX_SPRITE_LAYERS 16

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
	//8th bit set means a sprite uses Camera to Screen Matrix
	uint8_t  z;
} sprite;

void sprite_draw(sprite s, bool is_translucent);
void sprite_update(void);