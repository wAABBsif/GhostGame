#pragma once
#include "ecs_entity.h"

typedef void (*system_func)(void **components, entity_index entity);

typedef struct ecs_system
{
	system_func func;
} ecs_system;

void *system_retrieve_component(entity_index entity, component_type type, component_index *index);
void systems_init(void);
void systems_update(void);