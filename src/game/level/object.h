#pragma once
#include <stdint.h>

#include "core/vec2.h"

#define OBJECT_TYPE_NONE		0
#define OBJECT_TYPE_COUNT		1

typedef uint16_t object_type;

typedef struct object
{
	object_type type;
	int8_t x;
	int8_t y;
	union
	{
		char buffer[12];
	};
} object;

vec2 object_position_to_vec2(vec2 chunk_offset, int8_t x, int8_t y);