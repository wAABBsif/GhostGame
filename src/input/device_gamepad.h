#pragma once
#include "input_device.h"
#include "SDL3/SDL_gamepad.h"

typedef struct input_device_gamepad
{
	input_device base;
	SDL_Gamepad *gamepad;
} input_device_gamepad;

input_device_gamepad *device_gamepad_init(SDL_JoystickID id);

typedef enum input_gamepad_binding_type
{
	GAMEPAD_BINDING_NONE,
	GAMEPAD_BINDING_BUTTON,
	GAMEPAD_BINDING_TRIGGER,
	GAMEPAD_BINDING_AXIS_POSITIVE,
	GAMEPAD_BINDING_AXIS_NEGATIVE,
	GAMEPAD_BINDING_COUNT
} input_gamepad_binding_type;

#define BINDING_FROM_GAMEPAD(type, value) (value << 4 | type)

void gamepad_from_binding(input_binding binding, input_gamepad_binding_type *type, uint16_t *value);