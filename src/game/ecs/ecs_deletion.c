#include "ecs_deletion.h"
#include "ecs_entity.h"

void entity_deletion_update()
{
	entity_id i = 0;
	entity_id component_indices[COMPONENT_TYPE_COUNT];
	for (component_type j = 0; j < COMPONENT_TYPE_COUNT; j++)
		component_indices[j] = 0;

	while (i < entities_get_count())
	{
		if (!entity_get_tag(i, TAG_DELETION))
		{
			for (component_type j = 0; j < COMPONENT_TYPE_COUNT; j++)
			{
				if (component_exists(j, i))
					component_indices[j]++;
			}

			i++;
			continue;
		}

		for (component_type j = 1; j < COMPONENT_TYPE_COUNT; j++)
		{
			if (component_exists(j, i))
			{
				component_remove(j, i);
				component_indices[j]--;
			}
		}

		entities_remove(i);
	}
}
