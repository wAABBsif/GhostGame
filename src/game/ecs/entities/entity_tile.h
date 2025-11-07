#pragma once
#include "../ecs_entity.h"
#include "game/level/tile.h"
#include "gfx/texture.h"

#include "game/level/tile_atlas.h"

entity_index entity_tile_create(tile_atlas_h atlas, tile tile);