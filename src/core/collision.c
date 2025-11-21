#include "collision.h"

#include <assert.h>
#include <math.h>

#include "core/game_time.h"
#include "core/logging.h"
#include "game/ecs/ecs_system.h"
#include "game/ecs/components/component_collider.h"
#include "game/ecs/components/component_transform.h"
#include "SDL3/SDL_stdinc.h"

bool collision_check_point_box(const vec2 a, const vec2 b_pos, const vec2 b_radius)
{
	if (a.x < b_pos.x - b_radius.x)
		return false;

	if (a.x > b_pos.x + b_radius.x)
		return false;

	if (a.y < b_pos.y - b_radius.y)
		return false;

	if (a.y > b_pos.y + b_radius.y)
		return false;

	return true;
}

bool collision_check_point_circle(const vec2 a, const vec2 b_pos, const float b_radius)
{
	const vec2 v = vec2_sub(a, b_pos);
	return vec2_sqr_mag(v) <= b_radius * b_radius;
}

bool collision_check_box_box(const vec2 a_pos, const vec2 a_radius, const vec2 b_pos, const vec2 b_radius)
{
	if (a_pos.x - a_radius.x > b_pos.x + b_radius.x)
		return false;

	if (a_pos.x + a_radius.x < b_pos.x - b_radius.x)
		return false;

	if (a_pos.y - a_radius.y > b_pos.y + b_radius.y)
		return false;

	if (a_pos.y + a_radius.y < b_pos.y - b_radius.y)
		return false;

	return true;
}

bool collision_check_box_circle(const vec2 a_pos, const vec2 a_radius, const vec2 b_pos, const float b_radius)
{
	if (b_pos.x < a_pos.x)
	{
		if (a_pos.x - b_pos.x < b_radius + a_radius.x)
			return false;
	}
	else
	{
		if (b_pos.x - a_pos.x < b_radius + a_radius.x)
			return false;
	}

	if (b_pos.y < a_pos.y)
	{
		if (a_pos.y - b_pos.y < b_radius + a_radius.y)
			return false;
	}
	else
	{
		if (b_pos.y - a_pos.y < b_radius + a_radius.y)
			return false;
	}

	return true;
}

bool collision_check_circle_circle(const vec2 a_pos, const float a_radius, const vec2 b_pos, const float b_radius)
{
	const float sqr_dist = vec2_sqr_mag(vec2_sub(a_pos, b_pos));
	return sqr_dist <= a_radius * a_radius + b_radius * b_radius;
}

vec2 collision_get_normal_box_box(const vec2 a_pos, const vec2 a_radius, const vec2 b_pos, const vec2 b_radius)
{
	assert(collision_check_box_box(a_pos, a_radius, b_pos, b_radius));
	float penetration[4] =
	{
		(a_pos.x + a_radius.x) - (b_pos.x - b_radius.x), // left
		(b_pos.x + b_radius.x) - (a_pos.x - a_radius.x), // right
		(a_pos.y + a_radius.y) - (b_pos.y - b_radius.y), // down
		(b_pos.y + b_radius.y) - (a_pos.y - a_radius.y), // up
	};

	int max_index = 0;
	for (int i = 1; i < 4; i++)
	{
		if (penetration[i] > penetration[max_index])
			max_index = i;
	}

	switch (max_index)
	{
	case 0:
		return (vec2){1, 0};
	case 1:
		return (vec2){-1, 0};
	case 2:
		return (vec2){0, 1};
	case 3:
		return (vec2){0, -1};
	default:
		assert(false);
	}
}

entity_index collision_overlap_point(const vec2 point, const entity_index ignore_index)
{
	component_index transform_index = 0;
	component_index collider_index = 0;

	for (entity_index i = 0; i < entities_get_count(); i++)
	{
		const component_transform *c_transform = system_retrieve_component(i, COMPONENT_TYPE_TRANSFORM, &transform_index);
		const component_collider *c_collider = system_retrieve_component(i, COMPONENT_TYPE_COLLIDER, &collider_index);

		if (!c_collider)
			continue;

		assert(c_transform);

		if (c_collider->type == COLLIDER_TYPE_BOX)
		{
			if (!collision_check_point_box(point, c_transform->position, (vec2){c_collider->width_radius, c_collider->height_radius}))
				continue;
		}
		else if (c_collider->type == COLLIDER_TYPE_CIRCLE)
		{
			if (!collision_check_point_circle(point, c_transform->position, c_collider->radius))
				continue;
		}

		if (i == ignore_index)
			continue;

		return i;
	}

	return ENTITY_INDEX_INVALID;
}

collision_properties collision_run_collision(const component_transform *transform, const component_collider *collider, const entity_index ignore_index)
{
	component_index transform_index = 0;
	component_index collider_index = 0;

	for (entity_index i = 0; i < entities_get_count(); i++)
	{
		component_transform *c_transform = system_retrieve_component(i, COMPONENT_TYPE_TRANSFORM, &transform_index);
		component_collider *c_collider = system_retrieve_component(i, COMPONENT_TYPE_COLLIDER, &collider_index);

		if (!c_collider)
			continue;

		assert(c_transform);

		if (i == ignore_index)
			continue;

		vec2 normal;
		if (c_collider->type == COLLIDER_TYPE_BOX)
		{
			if (collider->type == COLLIDER_TYPE_BOX)
			{
				if (!collision_check_box_box(transform->position, (vec2){collider->width_radius, collider->height_radius}, c_transform->position, (vec2){c_collider->width_radius, c_collider->height_radius}))
					continue;
				normal = collision_get_normal_box_box(transform->position, (vec2){collider->width_radius, collider->height_radius}, c_transform->position, (vec2){c_collider->width_radius, c_collider->height_radius});
			}
			else if (collider->type == COLLIDER_TYPE_CIRCLE)
			{
				//if (!collision_check_box_circle(c_transform->position, (vec2){c_collider->width_radius, c_collider->height_radius}, transform->position, collider->radius))
					//continue;
				assert(false);
			}
			else
			{
				continue;
			}
		}
		else if (c_collider->type == COLLIDER_TYPE_CIRCLE)
		{
			if (collider->type == COLLIDER_TYPE_BOX)
			{
				// if (!collision_check_box_circle(transform->position, (vec2){collider->width_radius, collider->height_radius}, c_transform->position, c_collider->radius))
				// 	continue;
				assert(false);
			}
			else if (collider->type == COLLIDER_TYPE_CIRCLE)
			{
				// if (!collision_check_circle_circle(transform->position, collider->radius, c_transform->position, c_collider->radius))
				// 	continue;
				assert(false);
			}
			else
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		return (collision_properties){normal, c_transform, c_collider, i};
	}

	return (collision_properties){VEC2_NAN, NULL, NULL, ENTITY_INDEX_INVALID};
}