#include "system_collision.h"

#include "game/ecs/ecs_system.h"
#include "game/ecs/components/component_collider.h"
#include "game/ecs/components/component_position.h"

bool overlap_point(vec2 point, component_position *p, component_collider *c)
{
	return false;
}

entity_index system_collision_overlap_point(vec2 point, const entity_index obj_index)
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

	return UINT16_MAX;
}