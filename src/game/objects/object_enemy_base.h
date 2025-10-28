#pragma once
#include "core/vec2.h"
#include "game/object.h"

typedef struct object_enemy_base object_enemy_base;

typedef void (*object_enemy_command_func)(object_enemy_base *base, vec2 *move_dir, bool *shoot, bool *aux);
typedef void (*object_enemy_set_health_callback)(object_enemy_base *base, int32_t *health);

typedef struct object_enemy_base
{
	object_base base;
	vec2 position;
	vec2 velocity;
	float angle;
	int32_t _health;
	object_enemy_command_func get_command;
	object_enemy_set_health_callback set_health_callback;
} object_enemy_base;

uint16_t object_enemy_get_health(const object_enemy_base *base);
void object_enemy_set_health(object_enemy_base *base, int32_t value);