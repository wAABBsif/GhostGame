#pragma once
#include "gfx/color.h"
#include "gfx/lighting.h"

typedef struct component_light
{
	color    color;
	light_type type;
	int8_t priority;
	int8_t z;
} component_light;
