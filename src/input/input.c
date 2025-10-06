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
static input_device_type *s_device;
static input_device_funcs s_device_funcs[INPUT_DEVICE_COUNT];

void input_init(void)
{
	log_message("Initializing input...");
	s_device_funcs[INPUT_DEVICE_KBM] = device_kbm_get_funcs();
	s_device_funcs[INPUT_DEVICE_GAMEPAD] = device_gamepad_get_funcs();

	s_device = (input_device_type *)device_kbm_init();
}

void input_update(void)
{
	memcpy(s_prev_actions, s_curr_actions, sizeof(s_curr_actions));

	if (s_device)
	{
		s_device_funcs[*s_device].update(s_device);

		for (uint8_t a = 0; a < INPUT_ACTION_COUNT; a++)
			s_curr_actions[a] = s_device_funcs[*s_device].actions[a](s_device);
	}

	vec2 m = input_get_vector(INPUT_VECTOR_MOVEMENT);
	vec2 l = input_get_vector(INPUT_VECTOR_LOOK);

	log_message("{%d, %d, %d, (%f, %f), (%f, %f)", input_is_action_down(INPUT_ACTION_SHOOT), input_is_action_down(INPUT_ACTION_SWITCH), input_is_action_down(INPUT_ACTION_AUX), m.x, m.y, l.x, l.y);
}

void input_terminate(void)
{
	log_message("Terminating input...");
	if (s_device)
		s_device_funcs[*s_device].terminate(s_device);
}

void input_handle_sdl_event(const SDL_Event *event)
{
	if (event->type == SDL_EVENT_JOYSTICK_ADDED)
		SDL_OpenJoystick(event->jdevice.which);

	if (event->type == SDL_EVENT_JOYSTICK_BUTTON_DOWN)
	{
		const bool is_duplicate = s_device && *s_device == INPUT_DEVICE_GAMEPAD && SDL_GetGamepadID(((input_device_gamepad *)s_device)->gamepad) == event->jbutton.which;
		if (is_duplicate)
			return;

		if (s_device)
			s_device_funcs[*s_device].terminate(s_device);

		s_device = (input_device_type *)device_gamepad_init(event->jbutton.which);
	}

	if (event->type == SDL_EVENT_JOYSTICK_REMOVED)
	{
		if (s_device && *s_device == INPUT_DEVICE_GAMEPAD && SDL_GetGamepadID(((input_device_gamepad *)s_device)->gamepad) == event->jbutton.which)
			s_device_funcs[*s_device].terminate(s_device);
		s_device = NULL;
	}

	if (event->type == SDL_EVENT_KEY_DOWN)
	{
		const bool is_duplicate = s_device && *s_device == INPUT_DEVICE_KBM;
		if (is_duplicate)
			return;

		if (s_device)
			s_device_funcs[*s_device].terminate(s_device);

		s_device = (input_device_type *)device_kbm_init();
	}

	if (event->type == SDL_EVENT_KEYBOARD_REMOVED)
	{
		if (s_device && *s_device == INPUT_DEVICE_KBM)
			s_device_funcs[*s_device].terminate(s_device);
		s_device = NULL;
	}
}

vec2 input_get_vector(const input_vector_id id)
{
	if (id >= INPUT_VECTOR_COUNT)
	{
		log_warning("Attempted to access nonexistent input vector!");
		return VEC2_ZERO;
	}

	if (!s_device)
	{
		log_warning("No input device is active!");
		return VEC2_ZERO;
	}

	const vec2 result = s_device_funcs[*s_device].vectors[id](s_device);
	return vec2_clamp_mag(result, 1);
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