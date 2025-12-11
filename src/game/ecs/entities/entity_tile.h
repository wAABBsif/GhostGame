#pragma once
#include "../ecs_entity.h"
#include "../components/component_sprite.h"
#include "../components/component_transform.h"
#include "game/level/level.h"
#include "gfx/texture.h"

#include "game/level/tile_atlas.h"

entity_id entity_tile_create(uint8_t chunk_index);
void entity_tile_from_chunk(chunk_index chunk, uint16_t tile_index, component_transform *c_transform, component_sprite *c_sprite, component_collider *c_collider);
void entity_tile_from_atlas(tile_atlas_h atlas, component_sprite *c_sprite);