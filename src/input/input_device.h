#pragma once
#include "input.h"
#include "input_binding.h"

typedef enum input_device_type
{
	INPUT_DEVICE_KBM,
	INPUT_DEVICE_GAMEPAD,
	INPUT_DEVICE_COUNT
} input_device_type;

typedef float (*input_action_func)(const void *generic_device, input_action_id id);
typedef void (*input_update_func)(void *generic_device);
typedef void (*input_terminate_func)(void *generic_device);

typedef struct input_device
{
	input_device_type type;
	input_action_func action;
	input_update_func update;
	input_terminate_func terminate;
	input_binding_list binding_lists[INPUT_ACTION_COUNT];
} input_device;