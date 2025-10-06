#pragma once
#include "input.h"

#define INPUT_DEVICE_KBM 0
//#define INPUT_DEVICE_GAMEPAD 1
#define INPUT_DEVICE_COUNT 1

typedef struct input_device input_device;

typedef bool (*input_action_func)(void *generic_device);
typedef vec2 (*input_vector_func)(void *generic_device);
typedef void (*input_update_func)(void *generic_device);
typedef void (*input_terminate_func)(void *generic_device);

typedef struct input_device
{
	input_action_func actions[INPUT_ACTION_COUNT];
	input_vector_func vectors[INPUT_VECTOR_COUNT];
	input_update_func update;
	input_terminate_func terminate;
} input_device;