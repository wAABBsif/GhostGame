#include "device_gamepad.h"

#include <math.h>
#include <stdlib.h>

#include "core/logging.h"
#include "core/vec2.h"

static bool s_is_action_shoot(const void *generic_device)
{
	const input_device_gamepad *device = generic_device;

	return SDL_GetGamepadButton(device->gamepad, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER)
	|| (SDL_GetGamepadAxis(device->gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) > TRIGGER_THRESHOLD);
}

static bool s_is_action_switch(const void *generic_device)
{
	const input_device_gamepad *device = generic_device;

	return SDL_GetGamepadButton(device->gamepad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER)
	|| (SDL_GetGamepadAxis(device->gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) > TRIGGER_THRESHOLD);
}

static bool s_is_action_aux(const void *generic_device)
{
	const input_device_gamepad *device = generic_device;

	return SDL_GetGamepadButton(device->gamepad, SDL_GAMEPAD_BUTTON_SOUTH)
	|| SDL_GetGamepadButton(device->gamepad, SDL_GAMEPAD_BUTTON_EAST)
	|| SDL_GetGamepadButton(device->gamepad, SDL_GAMEPAD_BUTTON_WEST)
	|| SDL_GetGamepadButton(device->gamepad, SDL_GAMEPAD_BUTTON_NORTH);
}

static vec2 s_get_vector_movement(const void *generic_device)
{
	const input_device_gamepad *device = generic_device;

	vec2 result;

	result.x = SDL_GetGamepadAxis(device->gamepad, SDL_GAMEPAD_AXIS_LEFTX) / (float)INT16_MAX * LEFT_STICK_SENSITIVITY;
	if (fabsf(result.x) < LEFT_STICK_DEADZONE)
		result.x = 0;

	result.y = SDL_GetGamepadAxis(device->gamepad, SDL_GAMEPAD_AXIS_LEFTY) / (float)INT16_MAX * LEFT_STICK_SENSITIVITY;
	if (fabsf(result.y) < LEFT_STICK_DEADZONE)
		result.y = 0;

	if (SDL_GetGamepadButton(device->gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT))
		result.x += 1;

	if (SDL_GetGamepadButton(device->gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT))
		result.x -= 1;

	if (SDL_GetGamepadButton(device->gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP))
		result.y += 1;

	if (SDL_GetGamepadButton(device->gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN))
		result.y -= 1;

	result.y *= -1;
	return result;
}

static vec2 s_get_vector_look(const void *generic_device)
{
	const input_device_gamepad *device = generic_device;

	vec2 result;

	result.x = SDL_GetGamepadAxis(device->gamepad, SDL_GAMEPAD_AXIS_RIGHTX) / (float)INT16_MAX * RIGHT_STICK_SENSITIVITY;
	if (fabsf(result.x) < RIGHT_STICK_DEADZONE)
		result.x = 0;

	result.y = SDL_GetGamepadAxis(device->gamepad, SDL_GAMEPAD_AXIS_RIGHTY) / (float)INT16_MAX * RIGHT_STICK_SENSITIVITY;
	if (fabsf(result.y) < RIGHT_STICK_DEADZONE)
		result.y = 0;

	result.y *= -1;
	return result;
}

static void s_update(void *generic_device)
{
	input_device_gamepad *device = generic_device;
}

static void s_terminate(void *generic_device)
{
	input_device_gamepad *device = generic_device;

	SDL_Joystick *stick = SDL_GetGamepadJoystick(device->gamepad);
	SDL_CloseGamepad(device->gamepad);
	SDL_CloseJoystick(stick);

	free(device);
}

input_device_gamepad *device_gamepad_init(const SDL_JoystickID id)
{
	input_device_gamepad *result = malloc(sizeof(input_device_gamepad));
	result->type = INPUT_DEVICE_GAMEPAD;


	SDL_OpenJoystick(id);
	result->gamepad = SDL_OpenGamepad(id);
	if (result->gamepad == NULL)
		log_warning("Gamepad %i could not be initialized!", id);
	else
		log_message("Gamepad %i initialized", id);

	return result;
}

input_device_funcs device_gamepad_get_funcs(void)
{
	return (input_device_funcs)
	{
		s_is_action_shoot,
		s_is_action_switch,
		s_is_action_aux,
		s_get_vector_movement,
		s_get_vector_look,
		s_update,
		s_terminate,
	};
}