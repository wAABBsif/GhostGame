#pragma once
#include "core/vec2.h"
#include "game/ecs/ecs_entity.h"
#include "game/ecs/components/component_collider.h"
#include "game/ecs/components/component_transform.h"

typedef struct collision_properties
{
	entity_index entity;
	component_transform *transform;
	component_collider *collider;
} collision_properties;

collision_properties system_collision_overlap_point(vec2 point, entity_index obj_index);