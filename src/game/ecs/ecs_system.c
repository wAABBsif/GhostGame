#include "ecs_system.h"

#include <tgmath.h>

#include "ecs_entity.h"
#include "systems/system_deletion.h"
#include "systems/system_draw_lighting.h"
#include "systems/system_draw_sprites.h"
#include "systems/system_kinematics.h"

typedef void (*system_init_func)();
typedef void (*system_update_func)();

const system_init_func INIT_SYSTEMS[] = {};

const system_init_func UPDATE_SYSTEMS[] =
{
	system_deletion_update,
	system_kinematics_update,
	system_draw_sprites_update,
	system_draw_lighting_update
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
	for (int i = 0; i < sizeof(INIT_SYSTEMS) / sizeof(system_init_func); i++)
	{
		INIT_SYSTEMS[i]();
	}
}

void systems_update(void)
{
	for (int i = 0; i < sizeof(UPDATE_SYSTEMS) / sizeof(system_update_func); i++)
	{
		UPDATE_SYSTEMS[i]();
	}
}