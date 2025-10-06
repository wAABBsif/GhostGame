#pragma once
#include "input_device.h"
#include "SDL3/SDL_gamepad.h"

#define TRIGGER_THRESHOLD 20000
#define LEFT_STICK_DEADZONE 0.1
#define RIGHT_STICK_DEADZONE 0.1
#define LEFT_STICK_SENSITIVITY 1
#define RIGHT_STICK_SENSITIVITY 1

typedef struct input_device_gamepad
{
	input_device_type type;
	SDL_Gamepad *gamepad;
} input_device_gamepad;

input_device_gamepad *device_gamepad_init(SDL_JoystickID id);
input_device_funcs device_gamepad_get_funcs(void);