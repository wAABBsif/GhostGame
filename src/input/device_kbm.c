#include "device_kbm.h"

#include <stdlib.h>

#include "core/vec2.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"

static bool s_is_action_primary(void *generic_device)
{
	input_device_kbm device = *(input_device_kbm*)generic_device;
}

static bool s_is_action_secondary(void *generic_device)
{
	input_device_kbm device = *(input_device_kbm*)generic_device;
	return false;
}

static vec2 s_get_vector_movement(void *generic_device)
{
	const input_device_kbm device = *(input_device_kbm*)generic_device;
	vec2 result = VEC2_ZERO;

	if (device.keys[SDL_SCANCODE_LEFT] || device.keys[SDL_SCANCODE_A])
		result.x -= 1;

	if (device.keys[SDL_SCANCODE_RIGHT] || device.keys[SDL_SCANCODE_D])
		result.x += 1;

	if (device.keys[SDL_SCANCODE_DOWN] || device.keys[SDL_SCANCODE_S])
		result.y -= 1;

	if (device.keys[SDL_SCANCODE_UP] || device.keys[SDL_SCANCODE_W])
		result.y += 1;

	return result;
}

static vec2 s_get_vector_look(void *generic_device)
{
	input_device_kbm device = *(input_device_kbm*)generic_device;
	return VEC2_ZERO;
}

static void s_update(void *generic_device)
{
	input_device_kbm device = *(input_device_kbm*)generic_device;
}

static void s_terminate(void *generic_device)
{
	input_device_kbm device = *(input_device_kbm*)generic_device;
}

input_device_kbm *device_kbm_init()
{
	input_device_kbm *result = malloc(sizeof(input_device_kbm));

	result->base.actions[INPUT_ACTION_PRIMARY] = s_is_action_primary;
	result->base.actions[INPUT_ACTION_SECONDARY] = s_is_action_secondary;

	result->base.vectors[INPUT_VECTOR_MOVEMENT] = s_get_vector_movement;
	result->base.vectors[INPUT_VECTOR_LOOK] = s_get_vector_look;

	result->base.update = s_update;
	result->base.terminate = s_terminate;
	result->keys = SDL_GetKeyboardState(NULL);

	return result;
}