#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "ecs_component.h"

#define ECS_MAX_ENTITIES 8192
#define ENTITY_INDEX_INVALID UINT16_MAX

typedef uint16_t entity_index;

typedef struct ecs_entity
{
	uint64_t components;
} ecs_entity;

ecs_entity *entities_get();
entity_index entities_get_count();

entity_index entities_add();
void entities_remove(entity_index index);

void entity_queue_remove(entity_index index);

bool entity_has_component(entity_index index, component_type component);
void entity_enable_component(entity_index index, component_type type);
void *entity_add_component(component_type type);