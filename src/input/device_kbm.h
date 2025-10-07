#pragma once
#include "input_device.h"
#include "core/vec2.h"
#include "SDL3/SDL_mouse.h"

#define BINDING_FROM_KBM(type, value) (value << 4 | type)

typedef struct input_device_kbm
{
	input_device base;
	const bool *key_state;
	SDL_MouseButtonFlags mouse_state;
	vec2 mouse_offset;
} input_device_kbm;

input_device_kbm *device_kbm_init(void);

typedef enum input_kbm_binding_type
{
	KBM_BINDING_NONE,
	KBM_BINDING_KEY,
	KBM_BINDING_MOUSE_BUTTON,
	KBM_BINDING_COUNT
} input_kbm_binding_type;

void kbm_from_binding(input_binding binding, input_kbm_binding_type *type, uint16_t *value);