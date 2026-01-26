#include "system_kinematics.h"


#include "core/game_time.h"
#include "../ecs_component.h"
#include "../ecs_entity.h"
#include "../ecs_system.h"
#include "../components/component_transform.h"
#include "../components/component_kinematic_body.h"

void system_kinematics_update(const entity_id entity)
{
	component_transform *c_transform = component_get(COMPONENT_TYPE_TRANSFORM, entity);
	const component_kinematic_body *c_kinematics = component_get(COMPONENT_TYPE_KINEMATIC_BODY, entity);

	c_transform->position = vec2_add(c_transform->position, vec2_mul(c_kinematics->velocity, game_time_get_delta()));
}