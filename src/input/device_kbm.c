#include "device_kbm.h"

#include <stdlib.h>

#include "core/logging.h"
#include "core/vec2.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"

static float s_action(const void *generic_device, const input_action_id action)
{
	const input_device_kbm *device = generic_device;

	float result = 0;
	for (uint8_t b = 0; b < INPUT_BINDINGS_PER_LIST; b++)
	{
		float value = 0;
		input_kbm_binding_type type;
		uint16_t binding_value;

		kbm_from_binding(device->base.binding_lists[action].bindings[b], &type, &binding_value);
		switch (type)
		{
		case KBM_BINDING_KEY:
			value = device->key_state[binding_value] ? 1 : 0;
			break;
		case KBM_BINDING_MOUSE_BUTTON:
			value = device->mouse_state & SDL_BUTTON_MASK(binding_value) ? 1 : 0;
			break;
		default:
			value = 0;
			break;
		}

		if (value > result)
			result = value;
	}
	return result;
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
	result->base.type = INPUT_DEVICE_KBM;

	result->base.action = s_action;
	result->base.update = s_update;
	result->base.terminate = s_terminate;
	result->base.rumble = NULL;

	result->base.binding_lists[INPUT_ACTION_CONFIRM] = (input_binding_list){BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_RETURN), BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_RETURN2), 0, 0};
	result->base.binding_lists[INPUT_ACTION_CANCEL] = (input_binding_list){BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_ESCAPE), 0, 0, 0};
	result->base.binding_lists[INPUT_ACTION_SHOOT] = (input_binding_list){BINDING_FROM_KBM(KBM_BINDING_MOUSE_BUTTON, SDL_BUTTON_LEFT), 0, 0, 0};
	result->base.binding_lists[INPUT_ACTION_SWITCH] = (input_binding_list){BINDING_FROM_KBM(KBM_BINDING_MOUSE_BUTTON, SDL_BUTTON_RIGHT), 0, 0, 0};
	result->base.binding_lists[INPUT_ACTION_AUX] = (input_binding_list){BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_SPACE), 0, 0, 0};
	result->base.binding_lists[INPUT_ACTION_MOVE_L] = (input_binding_list){BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_A), BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_LEFT), 0, 0};
	result->base.binding_lists[INPUT_ACTION_MOVE_R] = (input_binding_list){BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_D), BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_RIGHT), 0, 0};
	result->base.binding_lists[INPUT_ACTION_MOVE_D] = (input_binding_list){BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_S), BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_DOWN), 0, 0};
	result->base.binding_lists[INPUT_ACTION_MOVE_U] = (input_binding_list){BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_W), BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_UP), 0, 0};
	//
	//
	//
	//
	result->base.binding_lists[INPUT_ACTION_PAUSE] = (input_binding_list){BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_RETURN), BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_RETURN2), BINDING_FROM_KBM(KBM_BINDING_KEY, SDL_SCANCODE_ESCAPE), 0};

	result->key_state = SDL_GetKeyboardState(NULL);
	result->mouse_state = SDL_GetMouseState(NULL, NULL);

	LOG_MESSAGE("Keyboard (+ Mouse) initialized");

	return result;
}

void kbm_from_binding(const input_binding binding, input_kbm_binding_type *type, uint16_t *value)
{
	*type = binding & 0b1111;
	*value = binding >> 4;
}