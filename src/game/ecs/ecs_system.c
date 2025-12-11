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

const ecs_system GAME_SYSTEMS[] =
{
	(ecs_system){COMPONENT_MASK(COMPONENT_TYPE_CONTROLLER), system_controller_update},
	(ecs_system){COMPONENT_MASK(COMPONENT_TYPE_TRANSFORM) | COMPONENT_MASK(COMPONENT_TYPE_SPRITE) | COMPONENT_MASK(COMPONENT_TYPE_COLLIDER) | COMPONENT_MASK(COMPONENT_TYPE_TILE), system_tiles_update},
	(ecs_system){COMPONENT_MASK(COMPONENT_TYPE_UPDATE), system_update_update},
	(ecs_system){COMPONENT_MASK(COMPONENT_TYPE_TRANSFORM) | COMPONENT_MASK(COMPONENT_TYPE_KINEMATIC_BODY) | COMPONENT_MASK(COMPONENT_TYPE_COLLIDER), system_dynamics_update},
	(ecs_system){COMPONENT_MASK(COMPONENT_TYPE_TRANSFORM) | COMPONENT_MASK(COMPONENT_TYPE_KINEMATIC_BODY), system_kinematics_update},
	(ecs_system){COMPONENT_MASK(COMPONENT_TYPE_TRANSFORM) | COMPONENT_MASK(COMPONENT_TYPE_SPRITE), system_draw_sprites_update},
	(ecs_system){COMPONENT_MASK(COMPONENT_TYPE_TRANSFORM) | COMPONENT_MASK(COMPONENT_TYPE_LIGHT), system_draw_lighting_update}
};

void *system_retrieve_component(const entity_id entity, const component_type type, component_index *index)
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
	for (uint16_t sys = 0; sys < systems_get_count(); sys++)
	{
		uint16_t component_count = 0;
		component_type component_types[COMPONENT_TYPE_COUNT];

		for (component_type type = 0; type < COMPONENT_TYPE_COUNT; type++)
		{
			if (!system_requires_component(GAME_SYSTEMS + sys, type))
				continue;

			component_types[component_count] = type;
			component_count++;
		}

		for (entity_id entity = 0; entity < entities_get_count(); entity++)
		{
			void *components[COMPONENT_TYPE_COUNT];
			if ((entity_get_component_mask(entity) & GAME_SYSTEMS[sys].components) != GAME_SYSTEMS[sys].components)
				continue;

			for (component_type type_index = 0; type_index < component_count; type_index++)
				components[type_index] = components_get_index(component_types[type_index], entity);
			GAME_SYSTEMS[sys].func(entity, components);
		}
	}
}

uint16_t systems_get_count(void)
{
	return sizeof(GAME_SYSTEMS) / sizeof(GAME_SYSTEMS[0]);
}

bool system_requires_component(const ecs_system *system, const component_type type)
{
	return system->components & COMPONENT_MASK(type);
}

uint16_t system_get_component_count(const ecs_system *system)
{
	uint16_t count = 0;
	for (component_type type = 0; type < COMPONENT_TYPE_COUNT; type++)
	{
		if (system_requires_component(system, type))
			count++;
	}
	return count;
}