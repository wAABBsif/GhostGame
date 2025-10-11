#include "input.h"

#include <math.h>

#include "input_device.h"
#include "core/vec2.h"

#include <string.h>

#include "device_gamepad.h"
#include "device_kbm.h"
#include "core/logging.h"

static bool s_curr_actions[INPUT_ACTION_COUNT];
static bool s_prev_actions[INPUT_ACTION_COUNT];
static input_device *s_device;

void input_init(void)
{
	log_message("Initializing input...");
	s_device = (input_device *)device_kbm_init();
}

void input_update(void)
{
	memcpy(s_prev_actions, s_curr_actions, sizeof(s_curr_actions));

	if (s_device)
	{
		s_device->update(s_device);

		for (uint8_t a = 0; a < INPUT_ACTION_COUNT; a++)
			s_curr_actions[a] = s_device->action(s_device, a) > 0.5;
	}
	//
	// log_message("[%f, %f", input_get_action(INPUT_ACTION_CONFIRM), input_get_action(INPUT_ACTION_CANCEL));
	// log_message("%f, %f, %f", input_get_action(INPUT_ACTION_SHOOT), input_get_action(INPUT_ACTION_SWITCH), input_get_action(INPUT_ACTION_AUX));
	// log_message("%f, %f", input_get_action(INPUT_ACTION_MOVE_R) - input_get_action(INPUT_ACTION_MOVE_L), input_get_action(INPUT_ACTION_MOVE_U) - input_get_action(INPUT_ACTION_MOVE_D));
	// log_message("%f, %f]", input_get_action(INPUT_ACTION_AIM_R) - input_get_action(INPUT_ACTION_AIM_L), input_get_action(INPUT_ACTION_AIM_U) - input_get_action(INPUT_ACTION_AIM_D));
	// log_message("");
}

void input_terminate(void)
{
	log_message("Terminating input...");
	if (s_device)
		s_device->terminate(s_device);
}

void input_handle_sdl_event(const SDL_Event *event)
{
	if (event->type == SDL_EVENT_JOYSTICK_ADDED)
		SDL_OpenJoystick(event->jdevice.which);

	if (event->type == SDL_EVENT_JOYSTICK_BUTTON_DOWN)
	{
		const bool is_duplicate = s_device && s_device->type == INPUT_DEVICE_GAMEPAD && SDL_GetGamepadID(((input_device_gamepad *)s_device)->gamepad) == event->jbutton.which;
		if (is_duplicate)
			return;

		if (s_device)
			s_device->terminate(s_device);

		s_device = (input_device *)device_gamepad_init(event->jbutton.which);
	}

	if (event->type == SDL_EVENT_JOYSTICK_REMOVED)
	{
		if (s_device && s_device->type == INPUT_DEVICE_GAMEPAD && SDL_GetGamepadID(((input_device_gamepad *)s_device)->gamepad) == event->jbutton.which)
			s_device->terminate(s_device);
		s_device = NULL;
	}

	if (event->type == SDL_EVENT_KEY_DOWN)
	{
		const bool is_duplicate = s_device && s_device->type == INPUT_DEVICE_KBM;
		if (is_duplicate)
			return;

		if (s_device)
			s_device->terminate(s_device);

		s_device = (input_device *)device_kbm_init();
	}

	if (event->type == SDL_EVENT_KEYBOARD_REMOVED)
	{
		if (s_device && s_device->type == INPUT_DEVICE_KBM)
			s_device->terminate(s_device);
		s_device = NULL;
	}
}

vec2 input_get_vector(const input_action_id left, const input_action_id right, const input_action_id down, const input_action_id up)
{
	vec2 result;
	result.x = input_get_action(right) - input_get_action(left);
	result.y = input_get_action(up) - input_get_action(down);

	return vec2_clamp_mag(result, 1);
}

float input_get_action(const input_action_id id)
{
	if (!s_device)
		return 0;

	const float result = s_device->action(s_device, id);
	if (result < 0)
		return 0;
	if (result > 1)
		return 1;

	return result;
}

bool input_is_action_down(const input_action_id id)
{
	return s_curr_actions[id];
}

bool input_was_action_pressed(const input_action_id id)
{
	return s_curr_actions[id] && !s_prev_actions[id];
}

bool input_was_action_released(const input_action_id id)
{
	return !s_curr_actions[id] && s_prev_actions[id];
}