#include "system_dynamics.h"

#include <assert.h>

#include "system_collision.h"
#include "core/game_time.h"
#include "core/vec2.h"
#include "game/ecs/ecs_entity.h"
#include "game/ecs/ecs_system.h"
#include "../components/component_transform.h"
#include "../components/component_dynamic_body.h"
#include "core/logging.h"
#include "game/ecs/components/component_kinematic_body.h"

vec2 component_dynamic_body_get_offset(const component_dynamic_body *component, const uint8_t collision_receiver)
{
	switch (collision_receiver)
	{
	case 0:
		return (vec2){component->width / 2, 0};
	case 1:
		return (vec2){0, component->height / 2};
	case 2:
		return (vec2){component->width / -2, 0};
	case 3:
		return (vec2){0, component->height / -2};
	default:
		return VEC2_NAN;
	}
}

void system_dynamics_update(void **components, entity_index entity)
{
	if (!entity_has_component(entity, COMPONENT_TYPE_DYNAMIC_BODY))
		return;

	assert(entity_has_component(entity, COMPONENT_TYPE_KINEMATIC_BODY));
	assert(entity_has_component(entity, COMPONENT_TYPE_TRANSFORM));

	component_transform *c_transform = components[COMPONENT_TYPE_TRANSFORM];
	component_kinematic_body *c_kinematics = components[COMPONENT_TYPE_KINEMATIC_BODY];
	component_dynamic_body *c_dynamics = components[COMPONENT_TYPE_DYNAMIC_BODY];

	for (uint8_t j = 0; j < COLLISION_RECEIVER_COUNT; j++)
	{
		const vec2 receiver_offset = component_dynamic_body_get_offset(c_dynamics, j);
		const vec2 receiver_pos = vec2_add(c_transform->position, receiver_offset);

		const collision_properties collision = system_collision_overlap_point(receiver_pos, entity);

		if (collision.transform != NULL)
		{
			const vec2 normal = vec2_normalize(vec2_neg(receiver_offset));
			const vec2 point = system_collision_get_closest_point_to_collider(c_transform->position, collision);
			const float point_to_dist = vec2_mag(vec2_sub(point, receiver_pos));
			c_transform->position = vec2_add(c_transform->position, vec2_mul(normal, point_to_dist));
			c_kinematics->velocity = vec2_project_on_plane(c_kinematics->velocity, normal);
		}
	}
}
