#pragma once
#include "game/ecs/ecs_component.h"

typedef void (*entity_update_func)(void **components, entity_index entity);

typedef struct component_update
{
	entity_update_func func;
} component_update;