#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "SDL3/SDL_events.h"

#define INPUT_MAX_RUMBLE_ENTRIES 8

typedef size_t rumble_id;
typedef struct vec2 vec2;

typedef enum input_action_id
{
	INPUT_ACTION_CONFIRM,
	INPUT_ACTION_CANCEL,
	INPUT_ACTION_SHOOT,
	INPUT_ACTION_SWITCH,
	INPUT_ACTION_AUX,
	INPUT_ACTION_MOVE_L,
	INPUT_ACTION_MOVE_R,
	INPUT_ACTION_MOVE_D,
	INPUT_ACTION_MOVE_U,
	INPUT_ACTION_AIM_L,
	INPUT_ACTION_AIM_R,
	INPUT_ACTION_AIM_D,
	INPUT_ACTION_AIM_U,
	INPUT_ACTION_PAUSE,
	INPUT_ACTION_COUNT
} input_action_id;

typedef struct rumble_entry
{
	float time;
	float strength;
	rumble_id id;
} rumble_entry;

void input_init(void);
void input_update(void);
void input_terminate(void);
void input_handle_sdl_event(const SDL_Event *event);

float input_get_action(input_action_id id);
vec2 input_get_vector(input_action_id left, input_action_id right, input_action_id down, input_action_id up);
bool input_is_action_down(input_action_id id);
bool input_was_action_pressed(input_action_id id);
bool input_was_action_released(input_action_id id);

rumble_id input_rumble_heavy(float time, float strength);
rumble_id input_rumble_light(float time, float strength);
bool input_is_rumble_active(rumble_id id);