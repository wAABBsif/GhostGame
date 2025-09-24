#pragma once
#include <stdint.h>
#include "color.h"

#define MAX_SPRITES 256

typedef struct sprite_vertex
{
	uint16_t x;
	uint16_t y;

	uint16_t texture_x;
	uint16_t texture_y;

	uint16_t texture_w;
	uint16_t texture_h;

	color    color;
	uint8_t  z;
	uint8_t  flags;
} sprite_vertex;

typedef struct sprite
{
	sprite_vertex vertices[4];
} sprite;

void sprite_renderer_init();
void sprite_renderer_terminate();

void sprite_renderer_draw();