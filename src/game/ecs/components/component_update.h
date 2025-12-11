#pragma once
#include "game/ecs/ecs_component.h"

typedef void (*entity_update_func)(entity_index entity, void **components);

typedef struct component_update
{
	entity_update_func func;
} component_update;