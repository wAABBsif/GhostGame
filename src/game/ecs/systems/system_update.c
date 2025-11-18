#include "system_update.h"

#include "game/ecs/ecs_entity.h"
#include "game/ecs/ecs_system.h"
#include "game/ecs/components/component_update.h"

void system_update_update(void **components, const entity_index entity)
{
	if (!entity_has_component(entity, COMPONENT_TYPE_UPDATE))
		return;

	component_update *c_update = components[COMPONENT_TYPE_UPDATE];
	c_update->func(components, entity);
}
