#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct vec2 vec2;

typedef uint8_t input_action_id;
#define INPUT_ACTION_PRIMARY 0
#define INPUT_ACTION_SECONDARY 1
#define INPUT_ACTION_COUNT 2

typedef uint8_t input_vector_id;
#define INPUT_VECTOR_MOVEMENT 0
#define INPUT_VECTOR_LOOK 1
#define INPUT_VECTOR_COUNT 2

void input_init(void);
void input_update(void);
void input_terminate(void);

bool input_is_action_down(input_action_id id);
bool input_was_action_pressed(input_action_id id);
bool input_was_action_released(input_action_id id);
vec2 input_get_vector(input_vector_id id);