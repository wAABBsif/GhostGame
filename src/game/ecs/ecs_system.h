#pragma once
#include "ecs_entity.h"

typedef void (*system_init_func)();
typedef void (*system_update_func)();

typedef struct ecs_system
{
	system_init_func init;
	system_update_func update;
} ecs_system;

void *system_retrieve_component(entity_index entity, component_type type, component_index *index);
void systems_init(void);
void systems_update(void);