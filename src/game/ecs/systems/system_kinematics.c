#include "system_kinematics.h"

#include <assert.h>

#include "core/game_time.h"
#include "../ecs_component.h"
#include "../ecs_entity.h"
#include "../ecs_system.h"
#include "../components/component_transform.h"
#include "../components/component_kinematic_body.h"

void system_kinematics_update(entity_id entity, void **components)
{
	component_transform *c_transform = components[0];
	component_kinematic_body *c_kinematics = components[1];

	c_transform->position = vec2_add(c_transform->position, vec2_mul(c_kinematics->velocity, game_time_get_delta()));
}