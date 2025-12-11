#include "system_controller.h"

#include "controllers/controller_player.h"
#include "core/logging.h"
#include "game/ecs/components/component_controller.h"

const controller_func CONTROLLER_FUNCS[] =
{
	controller_player
};

void system_controller_update(entity_index entity, void **components)
{
	component_controller *c_controller = components[0];
	CONTROLLER_FUNCS[c_controller->type](c_controller, entity);
}