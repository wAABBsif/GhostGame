#include "system_kinematics.h"

#include <assert.h>

#include "core/game_time.h"
#include "../ecs_component.h"
#include "../ecs_entity.h"
#include "../ecs_system.h"
#include "../components/component_transform.h"
#include "../components/component_kinematic_body.h"

void system_kinematics_update(void)
{
	component_index transform_index = 0;
	component_index kinematics_index = 0;

	for (entity_index i = 0; i < entities_get_count(); i++)
	{
		component_transform *c_transform = system_retrieve_component(i, COMPONENT_TYPE_TRANSFORM, &transform_index);
		component_kinematic_body *c_kinematics = system_retrieve_component(i, COMPONENT_TYPE_KINEMATIC_BODY, &kinematics_index);

		if (!c_kinematics)
			continue;

		assert(c_transform);

		c_transform->position = vec2_add(c_transform->position, vec2_mul(c_kinematics->velocity, game_time_get_delta()));
	}
}