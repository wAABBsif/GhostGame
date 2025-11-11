#include "system_update.h"

#include "game/ecs/ecs_entity.h"
#include "game/ecs/ecs_system.h"
#include "game/ecs/components/component_update.h"

void system_update_update(void)
{
	component_index component_indices[COMPONENT_TYPE_COUNT];
	for (component_type j = 0; j < COMPONENT_TYPE_COUNT; j++)
		component_indices[j] = 0;

	component_index update_index = 0;

	for (entity_index i = 0; i < entities_get_count(); i++)
	{
		for (component_type j = 0; j < COMPONENT_TYPE_COUNT; j++)
		{
			if (entity_has_component(i, j))
				component_indices[j]++;
		}

		component_update *c_update = system_retrieve_component(i, COMPONENT_TYPE_UPDATE, &update_index);
		if (!c_update)
			continue;

		c_update->func(component_indices);
	}
}
