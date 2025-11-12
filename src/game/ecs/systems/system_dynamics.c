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

void system_dynamics_update(void)
{
	component_index transform_index = 0;
	component_index kinematics_index = 0;
	component_index dynamics_index = 0;

	for (entity_index i = 0; i < entities_get_count(); i++)
	{
		component_transform *c_transform = system_retrieve_component(i, COMPONENT_TYPE_TRANSFORM, &transform_index);
		component_kinematic_body *c_kinematics = system_retrieve_component(i, COMPONENT_TYPE_KINEMATIC_BODY, &kinematics_index);
		component_dynamic_body *c_dynamics = system_retrieve_component(i, COMPONENT_TYPE_DYNAMIC_BODY, &dynamics_index);

		if (!c_dynamics)
			continue;

		assert(c_transform);
		assert(c_kinematics);

		for (uint8_t j = 0; j < COLLISION_RECEIVER_COUNT; j++)
		{
			if (c_dynamics->receivers[j].x_offset == 0 && c_dynamics->receivers[j].y_offset == 0)
				continue;

			const vec2 receiver_offset = (vec2){c_dynamics->receivers[j].x_offset, c_dynamics->receivers[j].y_offset};

			c_dynamics->receivers[j].received_index = system_collision_overlap_point(vec2_add(c_transform->position, receiver_offset), i);
			if (c_dynamics->receivers[j].received_index != ENTITY_INDEX_INVALID)
			{
				const vec2 normal = vec2_normalize(vec2_neg(receiver_offset));
				c_kinematics->velocity = vec2_project_on_plane(c_kinematics->velocity, normal);
				log_message("%f, %f", c_kinematics->velocity.x, c_kinematics->velocity.y);
			}
		}
	}
}
