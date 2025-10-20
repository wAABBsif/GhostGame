#pragma once
#include <stdint.h>

#define INPUT_BINDINGS_PER_LIST	4
typedef uint16_t input_binding;

typedef struct input_binding_list
{
	input_binding bindings[INPUT_BINDINGS_PER_LIST];
} input_binding_list;