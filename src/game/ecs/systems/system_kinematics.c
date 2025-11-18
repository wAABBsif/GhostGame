#include "system_kinematics.h"

#include <assert.h>

#include "core/game_time.h"
#include "../ecs_component.h"
#include "../ecs_entity.h"
#include "../ecs_system.h"
#include "../components/component_transform.h"
#include "../components/component_kinematic_body.h"

void system_kinematics_update(void **components, const entity_index entity)
{
	if (!entity_has_component(entity, COMPONENT_TYPE_KINEMATIC_BODY))
		return;

	assert(entity_has_component(entity, COMPONENT_TYPE_TRANSFORM));

	component_transform *c_transform = components[COMPONENT_TYPE_TRANSFORM];
	component_kinematic_body *c_kinematics = components[COMPONENT_TYPE_KINEMATIC_BODY];

	c_transform->position = vec2_add(c_transform->position, vec2_mul(c_kinematics->velocity, game_time_get_delta()));
}