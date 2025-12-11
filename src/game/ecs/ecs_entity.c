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
	if (entity_has_component(index, TAG_DELETION))
		return;
	entity_set_tag(index, TAG_DELETION, true);
}

component_mask entity_get_component_mask(const entity_index index)
{
	return s_entities[index].components;
}

bool entity_has_component(const entity_index index, const component_type component)
{
	return s_entities[index].components & COMPONENT_MASK(component);
}

void entity_enable_component(const entity_index index, const component_type type)
{
	ecs_entity *e = &s_entities[index];

	assert(!entity_has_component(index, type));
	e->components |= COMPONENT_MASK(type);
}

void *entity_add_component(const entity_index index, const component_type type)
{
	entity_enable_component(index, type);
	const component_index c_index = components_add(type, index);
	return components_get_index(type, index);
}

bool entity_get_tag(const entity_index index, const ecs_tag tag)
{
	return s_entities[index].tags & COMPONENT_MASK(tag);
}

void entity_set_tag(const entity_index index, const ecs_tag tag, bool const value)
{
	if (value)
		s_entities[index].tags |= COMPONENT_MASK(tag);
	else
		s_entities[index].tags &= ~COMPONENT_MASK(tag);
}