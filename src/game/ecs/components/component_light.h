#pragma once
#include "core/vec2.h"
#include "gfx/color.h"
#include "gfx/lighting.h"

typedef struct component_light
{
	vec2 size;

	color    color;

	light_type type;
	int8_t priority;
	int8_t z;
	uint8_t intensity_multiplier;
} component_light;
