#pragma once
#include <stdint.h>

#include "component_collider.h"
#include "game/ecs/ecs_entity.h"

#define COLLISION_RECEIVER_COUNT	4

typedef struct collision_receiver
{
	int8_t x_offset;
	int8_t y_offset;
	entity_index received_index;
} collision_receiver;

typedef struct component_dynamic_body
{
	collision_receiver receivers[COLLISION_RECEIVER_COUNT];
} component_dynamic_body;
