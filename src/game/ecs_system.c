#include "ecs_system.h"

#include "ecs_entity.h"
#include "components/component_position.h"
#include "systems/system_deletion.h"

const ecs_system SYSTEMS[] =
{
	(ecs_system){system_deletion_init, system_deletion_update}
};

void systems_init(void)
{
	entities_add();
	entity_queue_remove(0);
	component_position *p =  entity_add_component(COMPONENT_TYPE_POSITION);
	p->value = (vec2){3.0, 4.0};

	entities_add();
	p =  entity_add_component(COMPONENT_TYPE_POSITION);
	p->value = (vec2){5.0, 6.0};

	for (int i = 0; i < sizeof(SYSTEMS) / sizeof(ecs_system); i++)
	{
		SYSTEMS->init();
	}
}

void systems_update(void)
{
	for (int i = 0; i < sizeof(SYSTEMS) / sizeof(ecs_system); i++)
	{
		SYSTEMS->update();
	}
}