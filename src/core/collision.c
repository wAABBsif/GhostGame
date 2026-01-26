#include "collision.h"

#include <math.h>

#include "game_assert.h"
#include "core/game_time.h"
#include "core/logging.h"
#include "game/ecs/ecs_system.h"
#include "game/ecs/components/component_collider.h"
#include "game/ecs/components/component_transform.h"
#include "SDL3/SDL_stdinc.h"

bool collision_check_point_box(const vec2 point, const vec2 pos, const vec2 radius)
{
	return	point.x >= pos.x - radius.x
	&&		point.x <= pos.x + radius.x
	&&		point.y >= pos.y - radius.y
	&&		point.y <= pos.y + radius.y;
}

collision_data collision_overlap_point(const vec2 point, const entity_id ignore_index)
{
	entity_id transform_index = 0;
	entity_id collider_index = 0;

	for (entity_id i = 0; i < entities_get_count(); i++)
	{
		component_transform *c_transform = system_retrieve_component(i, COMPONENT_TYPE_TRANSFORM, &transform_index);
		component_collider *c_collider = system_retrieve_component(i, COMPONENT_TYPE_COLLIDER, &collider_index);

		if (!c_collider)
			continue;

		if (*(uint16_t *)c_collider == 0)
			continue;

		GAME_ASSERT(c_transform);

		if (!collision_check_point_box(point, c_transform->position, (vec2){c_collider->radius.x, c_collider->radius.y}))
			continue;

		if (i == ignore_index)
			continue;

		return (collision_data){c_transform, c_collider, i};
	}

	return (collision_data){NULL, NULL, ENTITY_ID_INVALID};
}

vec2 collision_get_closest_point(const vec2 point, const vec2 pos, const vec2 radius)
{
	const float x_offset = (pos.x - point.x) / radius.x;
	const float y_offset = (pos.y - point.y) / radius.y;

	if (fabsf(x_offset) >= fabsf(y_offset))
		return (vec2){pos.x - radius.x * x_offset / fabsf(x_offset), point.y};
	else
		return (vec2){point.x, pos.y - radius.y * y_offset / fabsf(y_offset)};
}