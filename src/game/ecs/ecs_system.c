#include "ecs_system.h"

#include <stddef.h>
#include <tgmath.h>

#include "ecs_entity.h"
#include "core/game_time.h"
#include "core/logging.h"
#include "systems/system_controller.h"
#include "systems/system_draw_lighting.h"
#include "systems/system_draw_sprites.h"
#include "systems/system_dynamics.h"
#include "systems/system_kinematics.h"
#include "systems/system_tiles.h"
#include "systems/system_update.h"

const ecs_system UPDATE_SYSTEMS[] =
{
	(ecs_system){system_controller_update},
	(ecs_system){system_tiles_update},
	(ecs_system){system_update_update},
	(ecs_system){system_dynamics_update},
	(ecs_system){system_kinematics_update},
	(ecs_system){system_draw_sprites_update},
	(ecs_system){system_draw_lighting_update}
};

void *system_retrieve_component(const entity_index entity, const component_type type, component_index *index)
{
	if (!entity_has_component(entity, type))
		return NULL;

	void *result = components_get_index(type, *index);
	(*index)++;
	return result;
}

void systems_init(void)
{

}

void systems_update(void)
{
	void *components[COMPONENT_TYPE_COUNT];
	for (component_type j = 0; j < COMPONENT_TYPE_COUNT; j++)
		components[j] = components_get(j);

	for (entity_index entity = 0; entity < entities_get_count(); entity++)
	{
		for (int system = 0; system < sizeof(UPDATE_SYSTEMS) / sizeof(system_func); system++)
		{
			UPDATE_SYSTEMS[system].func(components, entity);
		}

		for (component_type type = 0; type < COMPONENT_TYPE_COUNT; type++)
		{
			if (entity_has_component(entity, type))
				components[type] += components_get_size(type);
		}
	}
}