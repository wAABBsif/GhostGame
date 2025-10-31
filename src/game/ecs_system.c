#include "ecs_system.h"

#include "ecs_entity.h"
#include "components/component_position.h"
#include "core/game_time.h"
#include "core/logging.h"
#include "systems/system_deletion.h"
#include "systems/system_kinematics.h"

const ecs_system SYSTEMS[] =
{
	(ecs_system){system_deletion_init, system_deletion_update},
	(ecs_system){system_kinematics_init, system_kinematics_update}
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
	for (int i = 0; i < 4; i++)
	{
		entities_add();
		component_position *p =  entity_add_component(COMPONENT_TYPE_POSITION);
		p->value = (vec2){3.0, 4.0};

		if (i != 2)
		{
			component_position *v =  entity_add_component(COMPONENT_TYPE_VELOCITY);
			v->value = (vec2){100 - i, 0};
		}

		if (i == 3)
		{
			entity_queue_remove(1);
		}
	}

	for (int i = 0; i < sizeof(SYSTEMS) / sizeof(ecs_system); i++)
	{
		SYSTEMS[i].init();
	}
}

void systems_update(void)
{
	for (int i = 0; i < sizeof(SYSTEMS) / sizeof(ecs_system); i++)
	{
		SYSTEMS[i].update();
	}
}