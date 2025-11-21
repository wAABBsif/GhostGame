#include "system_dynamics.h"

#include <assert.h>
#include <stddef.h>

#include "core/game_time.h"
#include "core/vec2.h"
#include "game/ecs/ecs_entity.h"
#include "game/ecs/ecs_system.h"
#include "../components/component_transform.h"
#include "core/collision.h"
#include "core/logging.h"
#include "game/ecs/components/component_collider.h"
#include "game/ecs/components/component_kinematic_body.h"

void system_dynamics_update(void **components, entity_index entity)
{
	QUICK_PERFORMANCE_TIMER_START(dynamics);

	if (!entity_has_component(entity, COMPONENT_TYPE_COLLIDER) || !entity_has_component(entity, COMPONENT_TYPE_KINEMATIC_BODY))
		return;

	assert(entity_has_component(entity, COMPONENT_TYPE_TRANSFORM));

	component_transform *c_transform = components[COMPONENT_TYPE_TRANSFORM];
	component_kinematic_body *c_kinematics = components[COMPONENT_TYPE_KINEMATIC_BODY];
	component_collider *c_collider = components[COMPONENT_TYPE_COLLIDER];

	const collision_properties collision = collision_run_collision(c_transform, c_collider, entity);

	if (collision.transform != NULL)
	{
		//log_message("%f, %f", collision.normal.x, collision.normal.y);
		//const vec2 point = system_collision_get_closest_point_to_collider(c_transform->position, collision);
		//const float point_to_dist = vec2_mag(vec2_sub(point, receiver_pos));
		//c_transform->position = vec2_add(c_transform->position, vec2_mul(normal, point_to_dist));
		c_kinematics->velocity = vec2_project_on_plane(c_kinematics->velocity, collision.normal);
	}

	QUICK_PERFORMANCE_TIMER_END(dynamics);
}
