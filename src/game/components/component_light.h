#pragma once
#include "gfx/color.h"
#include "gfx/lighting.h"

typedef struct component_light
{
	color    color;
	int8_t z;
	light_type type;
} component_light;
