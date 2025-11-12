#pragma once
#include "game/ecs/ecs_component.h"

typedef void (*entity_update_func)(const component_index *components, entity_index index);

typedef struct component_update
{
	entity_update_func func;
} component_update;