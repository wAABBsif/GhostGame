#include "system_controller.h"

#include "controllers/controller_player.h"
#include "core/logging.h"
#include "game/ecs/components/component_controller.h"

const controller_func CONTROLLER_FUNCS[] =
{
	controller_player
};

void system_controller_update(void **components, const entity_index entity)
{
	if (!entity_has_component(entity, COMPONENT_TYPE_CONTROLLER))
		return;

	const component_controller *c_controller = components[COMPONENT_TYPE_CONTROLLER];
	CONTROLLER_FUNCS[c_controller->type](components);
}