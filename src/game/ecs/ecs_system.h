#pragma once
#include "ecs_entity.h"

void *system_retrieve_component(entity_index entity, component_type type, component_index *index);
void systems_init(void);
void systems_update(void);