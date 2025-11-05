#pragma once
#include <stdint.h>

#include "core/vec2.h"

#define COLLIDER_TYPE_NONE		0
#define COLLIDER_TYPE_BOX		1

typedef uint8_t collider_type;

typedef struct component_collider
{
	collider_type type;
	union
	{
		struct
		{
			uint16_t w;
			uint16_t h;
		} box;
	};
} component_collider;