#include "system_update.h"

#include "game/ecs/ecs_entity.h"
#include "game/ecs/ecs_system.h"
#include "game/ecs/components/component_update.h"

void system_update_update(entity_id entity)
{
	component_update *c_update = component_get(COMPONENT_TYPE_UPDATE, entity);

	c_update->func(entity);
}
