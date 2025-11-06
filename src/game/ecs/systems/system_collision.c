#include "system_collision.h"

#include "game/ecs/ecs_system.h"
#include "game/ecs/components/component_collider.h"
#include "game/ecs/components/component_position.h"

bool overlap_point(const vec2 point, component_position *p, component_collider *c)
{
	switch (c->type)
	{
		case COLLIDER_TYPE_BOX:
			const uint16_t w = c->box.w / 2;
			const uint16_t h = c->box.h / 2;
			return point.x > p->value.x - w && point.y > p->value.y - h && point.x < p->value.x + w && point.y < p->value.y + h;
		default:
			return false;

		// case COLLIDER_TYPE_CIRCLE:
		// 	const vec2 v = vec2_sub(point, p->value);
		// 	return vec2_sqr_mag(v) < c.circle.radius * c.circle.radius;
	}
}

entity_index system_collision_overlap_point(const vec2 point, const entity_index obj_index)
{
	component_index position_index = 0;
	component_index collider_index = 0;

	for (entity_index i = 0; i < entities_get_count(); i++)
	{
		component_position *c_position = system_retrieve_component(i, COMPONENT_TYPE_POSITION, &position_index);
		component_collider *c_collider = system_retrieve_component(i, COMPONENT_TYPE_COLLIDER, &collider_index);

		if (!c_position || !c_collider)
			continue;

		if (!overlap_point(point, c_position, c_collider))
			continue;

		if (i != obj_index)
			return i;
	}

	return ENTITY_INDEX_INVALID;
}