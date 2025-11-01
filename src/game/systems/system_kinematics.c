#include "system_kinematics.h"

#include "core/game_time.h"
#include "game/ecs_component.h"
#include "game/ecs_entity.h"
#include "game/ecs_system.h"
#include "game/components/component_position.h"
#include "game/components/component_velocity.h"

void system_kinematics_init(void)
{

}

void system_kinematics_update(void)
{
	component_index position_index = 0;
	component_index velocity_index = 0;

	for (entity_index i = 0; i < entities_get_count(); i++)
	{
		component_position *c_position = system_retrieve_component(i, COMPONENT_TYPE_POSITION, &position_index);
		component_velocity *c_velocity = system_retrieve_component(i, COMPONENT_TYPE_VELOCITY, &velocity_index);

		if (!c_position || !c_velocity)
			continue;

		c_position->value = vec2_add(c_position->value, vec2_mul(c_velocity->value, game_time_get_delta()));
	}
}