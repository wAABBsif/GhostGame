#include "system_update.h"

#include "game/ecs/ecs_entity.h"
#include "game/ecs/ecs_system.h"
#include "game/ecs/components/component_update.h"

void system_update_update(entity_id entity, void **components)
{
	component_update *c_update = components[0];

	c_update->func(entity, components);
}
