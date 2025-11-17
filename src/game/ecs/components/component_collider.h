#pragma once
#include <stdint.h>

#include "core/vec2.h"

#define COLLIDER_TYPE_NONE		0
#define COLLIDER_TYPE_BOX		1
#define COLLIDER_TYPE_CIRCLE	2

typedef uint8_t collider_type;

typedef struct component_collider
{
	collider_type type;
	union
	{
		struct
		{
			uint8_t width_radius;
			uint8_t height_radius;
		};
		struct
		{
			uint16_t radius;
		};
	};
} component_collider;