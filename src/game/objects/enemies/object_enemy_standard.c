#include "object_enemy_standard.h"

void object_enemy_standard_start(void *generic_object)
{
	object_enemy_standard *object = (object_enemy_standard *)generic_object;

	object->base.position = (vec2){256, 256};
	object->base.velocity = (vec2){0, 0};
	object->base.angle = 0;
	object->base._health = 100;
	object->base.get_command = NULL;
	object->base.set_health_callback = NULL;
}

void object_enemy_standard_update(void *generic_object)
{
	object_enemy_standard *object = (object_enemy_standard *)generic_object;
}

void object_enemy_standard_end(void *generic_object)
{
	object_enemy_standard *object = (object_enemy_standard *)generic_object;
}