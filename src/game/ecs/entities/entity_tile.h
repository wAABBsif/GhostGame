#pragma once
#include "../ecs_entity.h"
#include "gfx/texture.h"

#include "game/tiles/tile_atlas.h"

entity_index entity_tile_create(const tile_atlas_h atlas, const uint16_t tile_index, const vec2 pos, const int8_t z);