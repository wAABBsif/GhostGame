#pragma once
#include <stdint.h>

#include "component_collider.h"
#include "game/ecs/ecs_entity.h"

#define COLLISION_RECEIVER_COUNT	4

typedef struct component_dynamic_body
{
	uint16_t width;
	uint16_t height;
} component_dynamic_body;