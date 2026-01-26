#include "ecs_entity.h"

#include <string.h>

static entity_id s_entity_count;
static ecs_entity s_entities[ECS_MAX_ENTITIES];

ecs_entity *entities_get()
{
	return s_entities;
}

entity_id entities_get_count()
{
	return s_entity_count;
}

entity_id entities_add()
{
	const entity_id result = s_entity_count;
	s_entities[s_entity_count] = (ecs_entity){0};
	s_entity_count++;
	return result;
}

void entities_remove(const entity_id index)
{
	s_entity_count--;
	memmove(&s_entities[index], &s_entities[index + 1], sizeof(ecs_entity) * (s_entity_count - index));
}

void entity_queue_remove(const entity_id index)
{
	entity_set_tag(index, TAG_DELETION, true);
}

bool entity_get_tag(const entity_id index, const ecs_tag tag)
{
	return s_entities[index].tags & COMPONENT_MASK(tag);
}

void entity_set_tag(const entity_id index, const ecs_tag tag, bool const value)
{
	if (value)
		s_entities[index].tags |= COMPONENT_MASK(tag);
	else
		s_entities[index].tags &= ~COMPONENT_MASK(tag);
}