#pragma once
#include "core/vec2.h"
#include "game/ecs/ecs_entity.h"
#include "game/ecs/components/component_collider.h"
#include "game/ecs/components/component_transform.h"

typedef struct collision_data
{
	component_transform *transform;
	component_collider *collider;
	entity_id entity;
} collision_data;

bool collision_check_point_box(vec2 point, vec2 pos, vec2 radius);

collision_data collision_overlap_point(vec2 point, entity_id ignore_index);
vec2 collision_get_closest_point(vec2 point, vec2 pos, vec2 radius);