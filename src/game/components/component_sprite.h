#pragma once
#include "core/vec2.h"
#include "gfx/texture.h"
#include "gfx/color.h"

typedef struct component_sprite
{
	vec2 size;

	texture_h texture;

	uint16_t texture_x;
	uint16_t texture_y;
	uint16_t texture_w;
	uint16_t texture_h;

	color    color;
	int8_t z;
	bool use_camera_to_screen_matrix;
	bool draw_sorted;
} component_sprite;
