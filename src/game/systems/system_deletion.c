#include "system_deletion.h"

#include <string.h>

#include "game/ecs_component.h"
#include "game/ecs_entity.h"

void system_deletion_init(void)
{

}

void system_deletion_update(void)
{
	entity_index i = 0;
	component_index component_indices[COMPONENT_TYPE_COUNT];
	for (component_type j = 0; j < COMPONENT_TYPE_COUNT; j++)
		component_indices[j] = 0;

	while (i < entities_get_count())
	{
		if (!entity_has_component(i, COMPONENT_TYPE_DELETION_FLAG))
		{
			for (component_type j = 0; j < COMPONENT_TYPE_COUNT; j++)
			{
				if (entity_has_component(i, j))
					component_indices[j]++;
			}

			i++;
			continue;
		}

		for (component_type j = 1; j < COMPONENT_TYPE_COUNT; j++)
		{
			if (entity_has_component(i, j))
			{
				components_remove(j, component_indices[j]);
				component_indices[j]--;
			}
		}

		entities_remove(i);
	}
}