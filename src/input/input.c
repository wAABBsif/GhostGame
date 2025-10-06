#include "input.h"
#include "input_device.h"
#include "core/vec2.h"

#include <string.h>

#include "device_kbm.h"
#include "core/logging.h"

static bool s_curr_actions[INPUT_ACTION_COUNT];
static bool s_prev_actions[INPUT_ACTION_COUNT];
static input_device *s_devices[INPUT_DEVICE_COUNT];

void input_init(void)
{
	s_devices[0] = (input_device *)device_kbm_init();
}

void input_update(void)
{
	memcpy(s_prev_actions, s_curr_actions, sizeof(s_curr_actions));

	for (uint8_t d = 0; d < INPUT_DEVICE_COUNT; d++)
	{
		s_devices[d]->update(s_devices[d]);
	}

	for (uint8_t a = 0; a < INPUT_ACTION_COUNT; a++)
	{
		bool result = false;
		for (uint8_t d = 0; d < INPUT_DEVICE_COUNT; d++)
		{
			if (s_devices[d]->actions[a](s_devices[d]))
				result = true;
		}
		s_curr_actions[a] = result;
	}
}

void input_terminate(void)
{
	for (uint8_t d = 0; d < INPUT_DEVICE_COUNT; d++)
	{
		s_devices[d]->terminate(s_devices[d]);
	}
}

vec2 input_get_vector(const input_vector_id id)
{
	if (id >= INPUT_VECTOR_COUNT)
	{
		log_warning("Attempted to access nonexistent input vector!");
		return VEC2_ZERO;
	}

	vec2 result = VEC2_ZERO;
	for (uint8_t d = 0; d < INPUT_DEVICE_COUNT; d++)
	{
		const vec2 device_res = s_devices[d]->vectors[id](s_devices[d]);
		if (vec2_sqr_mag(device_res) > vec2_sqr_mag(result))
			result = device_res;
	}

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