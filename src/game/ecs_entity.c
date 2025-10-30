#include "ecs_entity.h"

#include <assert.h>
#include <string.h>

static entity_index s_entity_count;
static ecs_entity s_entities[ECS_MAX_ENTITIES];

ecs_entity *entities_get()
{
	return s_entities;
}

entity_index entities_get_count()
{
	return s_entity_count;
}

entity_index entities_add()
{
	const entity_index result = s_entity_count;
	s_entities[s_entity_count] = (ecs_entity){0};
	s_entity_count++;
	return result;
}

void entities_remove(const entity_index index)
{
	s_entity_count--;
	memmove(&s_entities[index], &s_entities[index + 1], sizeof(ecs_entity) * (s_entity_count - index));
}

void entity_queue_remove(const entity_index index)
{
	if (entity_has_component(index, COMPONENT_TYPE_DELETION_FLAG))
		return;
	entity_enable_component(index, COMPONENT_TYPE_DELETION_FLAG);
}

bool entity_has_component(const entity_index index, const component_type component)
{
	const ecs_entity *e = &s_entities[index];
	return e->components & COMPONENT_GET_MASK(component);
}

void entity_enable_component(entity_index index, const component_type type)
{
	ecs_entity *e = &s_entities[index];

	assert(!entity_has_component(index, type));
	e->components |= COMPONENT_GET_MASK(type);
}

void *entity_add_component(const component_type type)
{
	entity_enable_component(s_entity_count - 1, type);
	component_index c_index = components_add(type);
	return &components_get(type)[c_index * components_get_size(type)];
}