#pragma once
#include "core/vec2.h"
#include "game/ecs/ecs_entity.h"
#include "game/ecs/components/component_collider.h"
#include "game/ecs/components/component_transform.h"

typedef struct collision_data
{
	vec2 normal;
	component_transform *transform;
	component_collider *collider;
	entity_index entity;
} collision_properties;

bool collision_check_point_box(vec2 a, vec2 b_pos, vec2 b_radius);
bool collision_check_point_circle(vec2 a, vec2 b_pos, float b_radius);
bool collision_check_box_box(vec2 a_pos, vec2 a_radius, vec2 b_pos, vec2 b_radius);
bool collision_check_box_circle(vec2 a_pos, vec2 a_radius, vec2 b_pos, float b_radius);
bool collision_check_circle_circle(vec2 a_pos, float a_radius, vec2 b_pos, float b_radius);

vec2 collision_get_normal_box_box(vec2 a_pos, vec2 a_radius, vec2 b_pos, vec2 b_radius);
vec2 collision_get_normal_box_circle(vec2 a_pos, vec2 a_radius, vec2 b_pos, float b_radius);
vec2 collision_get_normal_circle_box(vec2 a_pos, float a_radius, vec2 b_pos, vec2 b_radius);
vec2 collision_get_normal_circle_circle(vec2 a_pos, float a_radius, vec2 b_pos, float b_radius);

entity_index collision_overlap_point(vec2 point, entity_index ignore_index);
collision_properties collision_run_collision(const component_transform *transform, const component_collider *collider, entity_index ignore_index);