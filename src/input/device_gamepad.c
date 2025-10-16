#include "device_gamepad.h"

#include <math.h>
#include <stdlib.h>

#include "core/logging.h"
#include "core/vec2.h"

const float GAMEPAD_DEADZONES[SDL_GAMEPAD_AXIS_COUNT] =
{
	0.15,
	0.15,
	0.15,
	0.15,
	0.15,
	0.15
};

static float s_action(const void *generic_device, const input_action_id action)
{
	const input_device_gamepad *device = generic_device;

	float result = 0;
	for (uint8_t b = 0; b < INPUT_BINDINGS_PER_LIST; b++)
	{
		float value = 0;
		input_gamepad_binding_type type;
		uint16_t binding_value;

		gamepad_from_binding(device->base.binding_lists[action].bindings[b], &type, &binding_value);
		switch (type)
		{
			case GAMEPAD_BINDING_BUTTON:
				value = SDL_GetGamepadButton(device->gamepad, binding_value);
				break;
			case GAMEPAD_BINDING_TRIGGER:
				value = SDL_GetGamepadAxis(device->gamepad, binding_value) / 32767.0f;
				break;
			case GAMEPAD_BINDING_AXIS_POSITIVE:
				value = fmaxf(0, SDL_GetGamepadAxis(device->gamepad, binding_value) / 32767.0f);
				break;
			case GAMEPAD_BINDING_AXIS_NEGATIVE:
				value = fmaxf(0, SDL_GetGamepadAxis(device->gamepad, binding_value) / -32768.0f);
				break;
			default:
				value = 0;
				break;
		}

		if (type == GAMEPAD_BINDING_AXIS_POSITIVE || type == GAMEPAD_BINDING_AXIS_NEGATIVE || type == GAMEPAD_BINDING_TRIGGER)
		{
			if (value < GAMEPAD_DEADZONES[binding_value])
				value = 0;
		}

		if (value > result)
			result = value;
	}
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

static void s_rumble(const void *generic_device, float heavy, float light)
{
	const input_device_gamepad *device = generic_device;

	if (heavy > 1)
		heavy = 1;

	if (light > 1)
		light = 1;

	SDL_RumbleGamepad(device->gamepad, heavy * 0xFFFF, light * 0xFFFF, 200);
}

input_device_gamepad *device_gamepad_init(const SDL_JoystickID id)
{
	input_device_gamepad *result = malloc(sizeof(input_device_gamepad));
	result->base.type = INPUT_DEVICE_GAMEPAD;

	result->base.action = s_action;
	result->base.update = s_update;
	result->base.terminate = s_terminate;
	result->base.rumble = s_rumble;

	result->base.binding_lists[INPUT_ACTION_CONFIRM] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_SOUTH), 0, 0, 0};
	result->base.binding_lists[INPUT_ACTION_CANCEL] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_EAST), 0, 0, 0};
	result->base.binding_lists[INPUT_ACTION_SHOOT] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER), BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_TRIGGER, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER), 0, 0};
	result->base.binding_lists[INPUT_ACTION_SWITCH] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER), BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_TRIGGER, SDL_GAMEPAD_AXIS_LEFT_TRIGGER), 0, 0};
	result->base.binding_lists[INPUT_ACTION_AUX] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_SOUTH), BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_EAST), BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_NORTH), BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_WEST)};
	result->base.binding_lists[INPUT_ACTION_MOVE_L] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_DPAD_LEFT), BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_AXIS_NEGATIVE, SDL_GAMEPAD_AXIS_LEFTX), 0, 0};
	result->base.binding_lists[INPUT_ACTION_MOVE_R] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_DPAD_RIGHT), BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_AXIS_POSITIVE, SDL_GAMEPAD_AXIS_LEFTX), 0, 0};
	result->base.binding_lists[INPUT_ACTION_MOVE_D] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_DPAD_DOWN), BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_AXIS_POSITIVE, SDL_GAMEPAD_AXIS_LEFTY), 0, 0};
	result->base.binding_lists[INPUT_ACTION_MOVE_U] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_DPAD_UP), BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_AXIS_NEGATIVE, SDL_GAMEPAD_AXIS_LEFTY), 0, 0};
	result->base.binding_lists[INPUT_ACTION_AIM_L] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_AXIS_NEGATIVE, SDL_GAMEPAD_AXIS_RIGHTX), 0, 0, 0};
	result->base.binding_lists[INPUT_ACTION_AIM_R] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_AXIS_POSITIVE, SDL_GAMEPAD_AXIS_RIGHTX), 0, 0, 0};
	result->base.binding_lists[INPUT_ACTION_AIM_D] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_AXIS_POSITIVE, SDL_GAMEPAD_AXIS_RIGHTY), 0, 0, 0};
	result->base.binding_lists[INPUT_ACTION_AIM_U] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_AXIS_NEGATIVE, SDL_GAMEPAD_AXIS_RIGHTY), 0, 0, 0};
	result->base.binding_lists[INPUT_ACTION_PAUSE] = (input_binding_list){BINDING_FROM_GAMEPAD(GAMEPAD_BINDING_BUTTON, SDL_GAMEPAD_BUTTON_START), 0, 0, 0};

	SDL_OpenJoystick(id);
	result->gamepad = SDL_OpenGamepad(id);
	if (result->gamepad == NULL)
		log_warning("Gamepad %i could not be initialized!", id);
	else
		log_message("Gamepad %i initialized", id);

	return result;
}

void gamepad_from_binding(const input_binding binding, input_gamepad_binding_type *type, uint16_t *value)
{
	*type = binding & 0b1111;
	*value = binding >> 4;
}