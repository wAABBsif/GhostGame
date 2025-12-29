#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "ecs_component.h"
#include "ecs_tag.h"

#define ECS_MAX_ENTITIES 4096
#define ENTITY_ID_INVALID UINT16_MAX

typedef struct ecs_entity
{
	tag_mask tags;
} ecs_entity;

ecs_entity *entities_get();
entity_id entities_get_count();

entity_id entities_add();
void entities_remove(entity_id index);

void entity_queue_remove(entity_id index);

bool entity_get_tag(entity_id index, ecs_tag tag);
void entity_set_tag(entity_id index, ecs_tag tag, bool value);