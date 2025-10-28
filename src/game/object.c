#include "object.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "core/logging.h"

#include "objects/object_camera_brain.h"
#include "objects/enemies/object_enemy_standard.h"

const object_func_entry OBJECT_FUNCS[] =
{
	{NULL, NULL, NULL},								//OBJECT_TYPE_NONE
	FUNC_ENTRY_FROM_NAME(object_camera_brain),		//OBJECT_TYPE_TEST
	{NULL, NULL, NULL},								//OBJECT_TYPE_ENEMY_BASE
	FUNC_ENTRY_FROM_NAME(object_enemy_standard)		//OBJECT_TYPE_ENEMY_STANDARD
};

static uint16_t s_object_count = 0;
static object_base *s_objects[MAX_OBJECT_COUNT];
static object_id s_object_current_id;

void object_init(void)
{
	object_add(sizeof(object_camera_brain), OBJECT_TYPE_CAMERA_BRAIN);
	object_add(sizeof(object_enemy_standard), OBJECT_TYPE_ENEMY_STANDARD);
}

void object_update(void)
{
	for (int16_t i = 0; i < s_object_count; i++)
	{
		if (s_objects[i]->_type & OBJECT_TYPE_DELETION_BIT)
			continue;

		if (OBJECT_FUNCS[s_objects[i]->_type].update != NULL)
			OBJECT_FUNCS[s_objects[i]->_type].update(s_objects[i]);
	}

	for (int16_t i = 0; i < s_object_count; i++)
	{
		if (s_objects[i]->_type & OBJECT_TYPE_DELETION_BIT)
		{
			OBJECT_FUNCS[object_get_type(s_objects[s_object_count])].end(s_objects[i]);

			free(s_objects[i]);
			memmove(&s_objects[i], &s_objects[i + 1], sizeof(s_objects[0]) * (s_object_count - i - 1));
			s_objects[i] = NULL;

			i--;
			s_object_count--;
		}
	}
}

void object_terminate(void)
{
	for (int16_t i = 0; i < s_object_count; i++)
	{
		OBJECT_FUNCS[object_get_type(s_objects[s_object_count])].end(s_objects[i]);
		free(s_objects[i]);
		s_object_count--;
	}
}

void *object_get_ptr(const object_id object)
{
	return s_objects[object];
}

object_id object_add(const size_t size, const object_type type)
{
	s_objects[s_object_count] = malloc(size);
	s_objects[s_object_count]->_type = type;
	s_objects[s_object_count]->_id = s_object_current_id;
	OBJECT_FUNCS[object_get_type(s_objects[s_object_count])].start(s_objects[s_object_count]);

	s_object_count++;
	s_object_current_id++;

	return s_objects[s_object_count - 1]->_id;
}

bool object_destroy(const object_id object)
{
	s_objects[object]->_type |= OBJECT_TYPE_DELETION_BIT;
	return true;
}

object_id object_get_id(const object_base *obj)
{
	return obj->_id;
}

object_type object_get_type(const object_base *obj)
{
	return obj->_type & OBJECT_TYPE_DELETION_MASK;
}