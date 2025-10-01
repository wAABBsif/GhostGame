#pragma once
#include <stdint.h>

#include "drawing.h"
#include "game/sprite.h"

typedef struct sprite_vertex
{
	uint16_t x;
	uint16_t y;

	uint16_t texture_x;
	uint16_t texture_y;

	color    color;
	uint8_t  z;
	uint8_t  texture_index;
} sprite_vertex;

typedef struct sprite_quad
{
	sprite_vertex vertices[4];
} sprite_quad;

void sprite_renderer_init();
void sprite_renderer_terminate();

uint8_t sprite_renderer_get_texture(texture_h h);

void sprite_renderer_add_sprite(sprite s);

void sprite_renderer_clear_frame();
void sprite_renderer_draw();