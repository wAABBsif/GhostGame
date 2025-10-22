#pragma once
#include "game/object.h"

typedef struct object_camera_brain
{
	object_base base;
} object_camera_brain;

void object_camera_brain_start(void *generic_object);
void object_camera_brain_update(void *generic_object);
void object_camera_brain_end(void *generic_object);