#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "SDL3/SDL_events.h"

typedef struct vec2 vec2;

typedef uint8_t input_action_id;
#define INPUT_ACTION_SHOOT 0
#define INPUT_ACTION_SWITCH 1
#define INPUT_ACTION_AUX 2
#define INPUT_ACTION_COUNT 3

typedef uint8_t input_vector_id;
#define INPUT_VECTOR_MOVEMENT 0
#define INPUT_VECTOR_LOOK 1
#define INPUT_VECTOR_COUNT 2

void input_init(void);
void input_update(void);
void input_terminate(void);
void input_handle_sdl_event(const SDL_Event *event);

bool input_is_action_down(input_action_id id);
bool input_was_action_pressed(input_action_id id);
bool input_was_action_released(input_action_id id);
vec2 input_get_vector(input_vector_id id);