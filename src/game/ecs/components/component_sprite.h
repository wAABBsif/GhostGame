#pragma once
#include "core/vec2.h"
#include "gfx/texture.h"
#include "gfx/color.h"

typedef struct component_sprite
{
	vec2 size;

	texture_h texture;

	vec2u16 texture_pos;
	vec2u16 texture_size;

	color    color;
	int8_t z;
	bool use_camera_to_screen_matrix;
	bool draw_sorted;
} component_sprite;
