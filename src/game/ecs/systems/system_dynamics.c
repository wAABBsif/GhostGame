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

static void s_system_dynamics_collision_response(component_transform *c_transform, component_kinematic_body *c_kinematics, const vec2 radius, const vec2 closest_point, const bool useX)
{
	vec2 normal;
	vec2 point;
	if (useX)
	{
		if (c_transform->position.x > closest_point.x)
		{
			normal = (vec2){1, 0};
			point = (vec2){c_transform->position.x - radius.x, c_transform->position.y};
		}
		else
		{
			normal = (vec2){-1, 0};
			point = (vec2){c_transform->position.x + radius.x, c_transform->position.y};
		}
	}
	else
	{
		if (c_transform->position.y > closest_point.y)
		{
			normal = (vec2){0, 1};
			point = (vec2){c_transform->position.x, c_transform->position.y - radius.y};
		}
		else
		{
			normal = (vec2){0, -1};
			point = (vec2){c_transform->position.x, c_transform->position.y + radius.y};
		}
	}
	c_kinematics->velocity = vec2_project_on_plane(c_kinematics->velocity, normal);
	c_transform->position = vec2_add(c_transform->position, vec2_mul(normal, vec2_mag(vec2_sub(point, closest_point))));
}

void system_dynamics_update(void **components, entity_index entity)
{
	QUICK_PERFORMANCE_TIMER_START(dynamics);

	if (!entity_has_component(entity, COMPONENT_TYPE_COLLIDER) || !entity_has_component(entity, COMPONENT_TYPE_KINEMATIC_BODY))
		return;

	assert(entity_has_component(entity, COMPONENT_TYPE_TRANSFORM));

	component_transform *c_transform = components[COMPONENT_TYPE_TRANSFORM];
	component_kinematic_body *c_kinematics = components[COMPONENT_TYPE_KINEMATIC_BODY];
	component_collider *c_collider = components[COMPONENT_TYPE_COLLIDER];

	const vec2 x_check_position = (vec2){c_transform->position.x + (float)c_collider->radius.x * (c_kinematics->velocity.x > 0 ? 1.0 : -1.0), c_transform->position.y};
	const collision_data x_collision_data = collision_overlap_point(x_check_position, entity);
	if (x_collision_data.transform != NULL)
	{
		const vec2 closest_point = collision_get_closest_point(x_check_position, x_collision_data.transform->position, (vec2){x_collision_data.collider->radius.x, x_collision_data.collider->radius.y});
		s_system_dynamics_collision_response(c_transform, c_kinematics, (vec2){c_collider->radius.x, c_collider->radius.y}, closest_point, true);
	}

	const vec2 y_check_position = (vec2){c_transform->position.x, c_transform->position.y + (float)c_collider->radius.y * (c_kinematics->velocity.y > 0 ? 1.0 : -1.0)};
	const collision_data y_collision_data = collision_overlap_point(y_check_position, entity);
	if (y_collision_data.transform != NULL)
	{
		const vec2 closest_point = collision_get_closest_point(y_check_position, y_collision_data.transform->position, (vec2){y_collision_data.collider->radius.x, y_collision_data.collider->radius.y});
		s_system_dynamics_collision_response(c_transform, c_kinematics, (vec2){c_collider->radius.x, c_collider->radius.y}, closest_point, false);
	}

	QUICK_PERFORMANCE_TIMER_END(dynamics);
}