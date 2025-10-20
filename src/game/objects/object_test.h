#pragma once
#include "game/object.h"

typedef struct object_test
{
	object_base base;
	float rotation;
} object_test;

void object_test_start(void *generic_object);
void object_test_update(void *generic_object);
void object_test_end(void *generic_object);