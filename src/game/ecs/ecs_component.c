#include "ecs_component.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ecs_entity.h"

#include "components/component_position.h"
#include "components/component_rotation.h"
#include "components/component_velocity.h"
#include "components/component_sprite.h"
#include "components/component_light.h"
#include "components/component_collider.h"

const register_component_entry COMPONENT_ENTRIES[] =
{
	(register_component_entry){0, 0},												//COMPONENT_TYPE_DELETION_FLAG
	(register_component_entry){sizeof(component_position), ECS_MAX_ENTITIES},		//COMPONENT_TYPE_POSITION
	(register_component_entry){sizeof(component_rotation), ECS_MAX_ENTITIES / 2},	//COMPONENT_TYPE_ROTATION
	(register_component_entry){sizeof(component_velocity), ECS_MAX_ENTITIES / 2},	//COMPONENT_TYPE_VELOCITY
	(register_component_entry){sizeof(component_sprite), ECS_MAX_ENTITIES},			//COMPONENT_TYPE_SPRITE
	(register_component_entry){sizeof(component_light), ECS_MAX_ENTITIES / 4},		//COMPONENT_TYPE_LIGHT
	(register_component_entry){sizeof(component_collider), ECS_MAX_ENTITIES},		//COMPONENT_TYPE_COLLIDER
};

static void *s_components[COMPONENT_TYPE_COUNT];
static component_index s_component_count[COMPONENT_TYPE_COUNT];

void components_init(void)
{
	size_t total_size = 0;

	for (int i = 0; i < COMPONENT_TYPE_COUNT; i++)
	{
		s_component_count[i] = 0;
		s_components[i] = (void*)total_size;
		total_size += COMPONENT_ENTRIES[i].size * COMPONENT_ENTRIES[i].count;
	}

	void *component_data = malloc(total_size);
	for (int i = 0; i < COMPONENT_TYPE_COUNT; i++)
	{
		s_components[i] += (size_t)component_data;
	}
}

void components_terminate(void)
{
	free(s_components[0]);

	for (int i = 0; i < COMPONENT_TYPE_COUNT; i++)
	{
		s_component_count[i] = 0;
		s_components[i] = NULL;
	}
}

component_index components_add(const component_type type)
{
	assert(type != COMPONENT_TYPE_DELETION_FLAG);

	uint16_t *count = &s_component_count[type];

	const component_index result = *count;
	(*count)++;
	assert(s_component_count[type] < COMPONENT_ENTRIES[type].count);
	return result;
}

void components_remove(const component_type type, const component_index index)
{
	assert(type != COMPONENT_TYPE_DELETION_FLAG);

	void *array = s_components[type];
	const uint16_t size = COMPONENT_ENTRIES[type].size;

	s_component_count[type]--;
	memmove(array + index * size, array + (index + 1) * size, size * (s_component_count[type] - index));
}

void *components_get(const component_type type)
{
	return s_components[type];
}

void *components_get_index(const component_type type, const component_index index)
{
	return components_get(type) + COMPONENT_ENTRIES[type].size * index;
}

uint16_t components_get_size(const component_type type)
{
	return COMPONENT_ENTRIES[type].size;
}

component_index components_get_count(const component_type type)
{
	return s_component_count[type];
}