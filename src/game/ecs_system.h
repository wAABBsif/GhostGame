#pragma once

typedef void (*system_init_func)();
typedef void (*system_update_func)();

typedef struct ecs_system
{
	system_init_func init;
	system_update_func update;
} ecs_system;

void systems_init(void);
void systems_update(void);