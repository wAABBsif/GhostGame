#include "system_collision.h"

#include <assert.h>

#include "core/game_time.h"
#include "core/logging.h"
#include "game/ecs/ecs_system.h"
#include "game/ecs/components/component_collider.h"
#include "game/ecs/components/component_dynamic_body.h"
#include "game/ecs/components/component_transform.h"
#include "SDL3/SDL_stdinc.h"

bool overlap_point(const vec2 point, const component_transform *t, const component_collider *c)
{
	switch (c->type)
	{
		case COLLIDER_TYPE_BOX:
			const uint16_t w = c->width_radius;
			const uint16_t h = c->height_radius;
			return point.x >= t->position.x - w && point.y >= t->position.y - h && point.x <= t->position.x + w && point.y <= t->position.y + h;
		case COLLIDER_TYPE_CIRCLE:
			const vec2 v = vec2_sub(point, t->position);
			return vec2_sqr_mag(v) <= c->radius * c->radius;
		default:
			return false;
	}
}

vec2 system_collision_get_closest_point_to_collider(const vec2 point, const collision_properties collision)
{
	switch (collision.collider->type)
	{
	case COLLIDER_TYPE_BOX:
		vec2 closest_point_xy;
		closest_point_xy.x = SDL_clamp(point.x, collision.transform->position.x - collision.collider->width_radius, collision.transform->position.x + collision.collider->width_radius);
		closest_point_xy.y = SDL_clamp(point.y, collision.transform->position.y - collision.collider->height_radius, collision.transform->position.y + collision.collider->height_radius);

		const vec2 points[4] =
		{
			(vec2){collision.transform->position.x - collision.collider->width_radius, closest_point_xy.y},
			(vec2){closest_point_xy.x, collision.transform->position.y - collision.collider->height_radius},
			(vec2){collision.transform->position.x + collision.collider->width_radius, closest_point_xy.y},
			(vec2){closest_point_xy.x, collision.transform->position.y + collision.collider->height_radius},
		};

		uint8_t closest_point = 0;
		for (int i = 1; i < 4; i++)
		{
			const float closest_sqr_mag = vec2_sqr_mag(vec2_sub(point, points[closest_point]));
			const float current_sqr_mag = vec2_sqr_mag(vec2_sub(point, points[i]));
			if (closest_sqr_mag > current_sqr_mag)
				closest_point = i;
		}

		return points[closest_point];
	case COLLIDER_TYPE_CIRCLE:
		const vec2 dir = vec2_normalize(vec2_sub(point, collision.transform->position));
		return vec2_add(collision.transform->position, vec2_mul(dir, collision.collider->radius));
	default:
		return VEC2_NAN;
	}
}

collision_properties system_collision_overlap_point(const vec2 point, const entity_index obj_index)
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

		if (!overlap_point(point, c_transform, c_collider))
			continue;

		if (i == obj_index)
			continue;

		return (collision_properties){c_transform, c_collider};
	}

	return (collision_properties){NULL, NULL};
}