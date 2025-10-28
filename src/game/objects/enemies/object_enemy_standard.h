#pragma once
#include "game/objects/object_enemy_base.h"

typedef struct object_enemy_standard
{
	object_enemy_base base;
} object_enemy_standard;

void object_enemy_standard_start(void *generic_object);
void object_enemy_standard_update(void *generic_object);
void object_enemy_standard_end(void *generic_object);