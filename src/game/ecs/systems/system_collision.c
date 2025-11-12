#include "system_collision.h"

#include <assert.h>

#include "game/ecs/ecs_system.h"
#include "game/ecs/components/component_collider.h"
#include "game/ecs/components/component_dynamic_body.h"
#include "game/ecs/components/component_transform.h"

bool overlap_point(const vec2 point, component_transform *t, component_collider *c)
{
	switch (c->type)
	{
		case COLLIDER_TYPE_BOX:
			const uint16_t w = c->box.w / 2;
			const uint16_t h = c->box.h / 2;
			return point.x >= t->position.x - w && point.y >= t->position.y - h && point.x <= t->position.x + w && point.y <= t->position.y + h;
		case COLLIDER_TYPE_CIRCLE:
			const vec2 v = vec2_sub(point, t->position);
			return vec2_sqr_mag(v) <= c->circle.radius * c->circle.radius;
		default:
			return false;
	}
}

entity_index system_collision_overlap_point(const vec2 point, const entity_index obj_index)
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

		return i;
	}

	return ENTITY_INDEX_INVALID;
}