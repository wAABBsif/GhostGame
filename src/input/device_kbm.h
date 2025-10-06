#pragma once
#include "input_device.h"
#include "core/vec2.h"
#include "SDL3/SDL_mouse.h"

typedef struct input_device_kbm
{
	input_device_type type;
	const bool *key_state;
	SDL_MouseButtonFlags mouse_state;
	vec2 mouse_offset;
} input_device_kbm;

input_device_kbm *device_kbm_init(void);
input_device_funcs device_kbm_get_funcs(void);