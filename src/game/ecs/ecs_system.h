#pragma once
#include "ecs_entity.h"

typedef void (*system_func)(entity_id entity, void **components);

typedef struct ecs_system
{
	component_mask components;
	system_func func;
} ecs_system;

void *system_retrieve_component(entity_id entity, component_type type, component_index *index);
void systems_init(void);
void systems_update(void);
uint16_t systems_get_count(void);

bool system_requires_component(const ecs_system *system, component_type type);
uint16_t system_get_component_count(const ecs_system *system);