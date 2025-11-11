#include "system_kinematics.h"

#include <assert.h>

#include "core/game_time.h"
#include "../ecs_component.h"
#include "../ecs_entity.h"
#include "../ecs_system.h"
#include "../components/component_transform.h"
#include "../components/component_velocity.h"

void system_kinematics_update(void)
{
	component_index transform_index = 0;
	component_index velocity_index = 0;

	for (entity_index i = 0; i < entities_get_count(); i++)
	{
		component_transform *c_transform = system_retrieve_component(i, COMPONENT_TYPE_TRANSFORM, &transform_index);
		component_velocity *c_velocity = system_retrieve_component(i, COMPONENT_TYPE_VELOCITY, &velocity_index);

		if (!c_velocity)
			continue;

		assert(c_transform);

		c_transform->position = vec2_add(c_transform->position, vec2_mul(c_velocity->value, game_time_get_delta()));
	}
}