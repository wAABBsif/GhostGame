#include "ecs_component.h"

#include <stdlib.h>
#include <string.h>

#include "ecs_entity.h"

#include "components/component_transform.h"
#include "components/component_kinematic_body.h"
#include "components/component_sprite.h"
#include "components/component_light.h"
#include "components/component_collider.h"
#include "components/component_tile.h"
#include "components/component_update.h"
#include "components/component_controller.h"
#include "components/component_movement_properties.h"
#include "core/game_assert.h"
#include "game/level/level.h"

entity_id s_sparse_components[ECS_MAX_ENTITIES][COMPONENT_TYPE_COUNT];
void *s_dense_components[COMPONENT_TYPE_COUNT];

typedef struct register_component_entry
{
	uint16_t size;
	entity_id count;
} register_component_entry;

typedef struct component_set
{
	entity_id *sparse;
	void *dense;
	entity_id entity_count;
} component_set;

const register_component_entry COMPONENT_ENTRIES[] =
{
	(register_component_entry){sizeof(component_transform), ECS_MAX_ENTITIES},		//COMPONENT_TYPE_TRANSFORM
	(register_component_entry){sizeof(component_kinematic_body), 256},				//COMPONENT_TYPE_KINEMATIC_BODY
	(register_component_entry){sizeof(component_sprite), ECS_MAX_ENTITIES},			//COMPONENT_TYPE_SPRITE
	(register_component_entry){sizeof(component_light), 64},						//COMPONENT_TYPE_LIGHT
	(register_component_entry){sizeof(component_collider), ECS_MAX_ENTITIES},		//COMPONENT_TYPE_COLLIDER
	(register_component_entry){sizeof(component_tile), LEVEL_CHUNK_SIZE * 4},		//COMPONENT_TYPE_TILE
	(register_component_entry){sizeof(component_update), ECS_MAX_ENTITIES / 2},		//COMPONENT_TYPE_UPDATE
	(register_component_entry){sizeof(component_controller), 256},					//COMPONENT_TYPE_CONTROLLER
	(register_component_entry){sizeof(component_movement_properties), 256},			//COMPONENT_TYPE_MOVEMENT_PROPERTIES
};

static component_set s_component_set[COMPONENT_TYPE_COUNT];

void components_init(void)
{
	size_t total_size = 0;

	for (int i = 0; i < COMPONENT_TYPE_COUNT; i++)
	{
		s_component_set[i].dense = (void*)total_size;
		total_size += COMPONENT_ENTRIES[i].size * ECS_MAX_ENTITIES;
		s_component_set[0].entity_count = 0;
	}

	s_component_set[0].dense = malloc(total_size);
	s_component_set[0].sparse = malloc(sizeof(entity_id) * ECS_MAX_ENTITIES * COMPONENT_TYPE_COUNT);
	s_component_set[0].entity_count = 0;

	for (size_t i = 0; i < ECS_MAX_ENTITIES * COMPONENT_TYPE_COUNT; i++)
		s_component_set[0].sparse[i] = -1;

	for (int i = 1; i < COMPONENT_TYPE_COUNT; i++)
	{
		s_component_set[i].dense += (size_t)s_component_set[0].dense;
		s_component_set[i].sparse = s_component_set[0].sparse + ECS_MAX_ENTITIES * i;
	}
}

void components_terminate(void)
{
	free(s_component_set[0].dense);
	free(s_component_set[0].sparse);

	for (int i = 0; i < COMPONENT_TYPE_COUNT; i++)
	{
		s_component_set[i].dense = NULL;
		s_component_set[i].sparse = NULL;
		s_component_set[i].entity_count = 0;
	}
}

void *component_add(const component_type type, const entity_id index)
{
	GAME_ASSERT(index < ECS_MAX_ENTITIES);
	GAME_ASSERT(type < COMPONENT_TYPE_COUNT);
	GAME_ASSERT(s_component_set[type].sparse[index] < 0);

	s_component_set[type].sparse[index] = s_component_set[type].entity_count;
	void *result = s_component_set[type].dense + s_component_set[type].entity_count * COMPONENT_ENTRIES[type].size;
	s_component_set[type].entity_count++;

	return result;
}

void component_remove(const component_type type, const entity_id index)
{
	GAME_ASSERT(index < ECS_MAX_ENTITIES);
	GAME_ASSERT(type < COMPONENT_TYPE_COUNT);
	GAME_ASSERT(s_component_set[type].sparse[index] >= 0);

	memmove(component_get(type, index), component_get(type, s_component_set[type].entity_count), COMPONENT_ENTRIES[type].size);
	s_component_set[type].sparse[index] = -1;
	s_component_set[type].entity_count--;
}

bool component_exists(const component_type type, const entity_id index)
{
	GAME_ASSERT(index < ECS_MAX_ENTITIES);
	GAME_ASSERT(type < COMPONENT_TYPE_COUNT);

	const entity_id dense_idx = s_component_set[type].sparse[index];
	return dense_idx >= 0;
}


void *component_get(const component_type type, const entity_id index)
{
	GAME_ASSERT(index < ECS_MAX_ENTITIES);
	GAME_ASSERT(type < COMPONENT_TYPE_COUNT);

	const entity_id dense_idx = s_component_set[type].sparse[index];
	return s_component_set[type].dense + dense_idx * COMPONENT_ENTRIES[type].size;
}

uint16_t component_get_size(const component_type type)
{
	return COMPONENT_ENTRIES[type].size;
}