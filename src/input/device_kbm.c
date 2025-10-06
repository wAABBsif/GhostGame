#include "device_kbm.h"

#include <stdlib.h>

#include "core/logging.h"
#include "core/vec2.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"

static bool s_is_action_shoot(const void *generic_device)
{
	const input_device_kbm *device = generic_device;
	return device->mouse_state & SDL_BUTTON_LMASK;
}

static bool s_is_action_switch(const void *generic_device)
{
	const input_device_kbm *device = generic_device;
	return device->mouse_state & SDL_BUTTON_RMASK;
}

static bool s_is_action_aux(const void *generic_device)
{
	const input_device_kbm *device = generic_device;
	return device->key_state[SDL_SCANCODE_SPACE] || device->key_state[SDL_SCANCODE_E];
}

static vec2 s_get_vector_movement(const void *generic_device)
{
	const input_device_kbm *device = generic_device;
	vec2 result = VEC2_ZERO;

	if (device->key_state[SDL_SCANCODE_LEFT] || device->key_state[SDL_SCANCODE_A])
		result.x -= 1;

	if (device->key_state[SDL_SCANCODE_RIGHT] || device->key_state[SDL_SCANCODE_D])
		result.x += 1;

	if (device->key_state[SDL_SCANCODE_DOWN] || device->key_state[SDL_SCANCODE_S])
		result.y -= 1;

	if (device->key_state[SDL_SCANCODE_UP] || device->key_state[SDL_SCANCODE_W])
		result.y += 1;

	return result;
}

static vec2 s_get_vector_look(const void *generic_device)
{
	const input_device_kbm *device = generic_device;
	return vec2_normalize(device->mouse_offset);
}

static void s_update(void *generic_device)
{
	input_device_kbm *device = generic_device;
	float x, y;
	device->mouse_state = SDL_GetMouseState(&x, &y);
	device->mouse_offset.x = x;
	device->mouse_offset.y = y;
}

static void s_terminate(void *generic_device)
{
	input_device_kbm *device = generic_device;
	device->key_state = NULL;

	free(device);
}

input_device_kbm *device_kbm_init(void)
{
	input_device_kbm *result = malloc(sizeof(input_device_kbm));
	result->type = INPUT_DEVICE_KBM;

	result->key_state = SDL_GetKeyboardState(NULL);
	result->mouse_state = SDL_GetMouseState(NULL, NULL);

	log_message("Keyboard (+ Mouse) initialized");

	return result;
}

input_device_funcs device_kbm_get_funcs()
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