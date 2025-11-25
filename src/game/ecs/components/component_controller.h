#pragma once
#include <stdint.h>
#include "core/vec2.h"

typedef uint16_t controller_type;

#define CONTROLLER_TYPE_PLAYER	0

typedef void (*controller_func)(void **components);

typedef struct component_controller
{
	controller_type type;

	bool shoot_down : 1;
	bool shoot_pressed : 1;
	bool shoot_released : 1;

	bool aux_down : 1;
	bool aux_pressed : 1;
	bool aux_released : 1;

	vec2 move;
	vec2 aim;
} component_controller;