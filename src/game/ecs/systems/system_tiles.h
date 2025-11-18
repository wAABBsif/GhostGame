#pragma once
#include "../ecs_entity.h"

void system_tiles_load_level(void);
void system_tiles_begin_frame(void);
void system_tiles_update(void **components, entity_index entity);