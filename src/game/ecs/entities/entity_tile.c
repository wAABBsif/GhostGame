#include "entity_tile.h"

#include <math.h>

#include "core/vec2.h"
#include "../components/component_transform.h"
#include "../components/component_sprite.h"
#include "game/ecs/components/component_tile.h"
#include "game/level/tile.h"

entity_index entity_tile_create(const uint8_t chunk_index)
{
	const entity_index result = entities_add();

	component_transform *c_pos =  entity_add_component(COMPONENT_TYPE_TRANSFORM);
	c_pos->position = VEC2_ZERO;
	c_pos->rotation = 0;

	component_sprite *c_spr =  entity_add_component(COMPONENT_TYPE_SPRITE);
	c_spr->size = VEC2_ZERO;
	c_spr->texture = HASH_INVALID;
	c_spr->texture_x = 0;
	c_spr->texture_y = 0;
	c_spr->texture_w = 0;
	c_spr->texture_h = 0;
	c_spr->color = COLOR_WHITE;
	c_spr->z = 0;
	c_spr->use_camera_to_screen_matrix = false;
	c_spr->draw_sorted = false;

	component_collider *c_coll = entity_add_component(COMPONENT_TYPE_COLLIDER);
	c_coll->type = COLLIDER_TYPE_NONE;
	c_coll->width_radius = 0;
	c_coll->height_radius = 0;

	component_tile *c_tile = entity_add_component(COMPONENT_TYPE_TILE);
	c_tile->chunk_index = chunk_index;

	return result;
}

void entity_tile_from_chunk(const chunk_index chunk, const uint16_t tile_index, component_transform *c_transform, component_sprite *c_sprite, component_collider *c_collider)
{
	const tile t = level_get_tile_chunk(chunk)->tiles[tile_index];
	const atlas_tile atlas_entry = tile_atlas_get_tile(level_get_tile_atlas(), t.atlas_index);

	c_transform->position = level_chunk_position_to_vec2(*level_get_chunk_position(chunk));
	c_transform->position = vec2_sub(c_transform->position, (vec2){LEVEL_CHUNK_WIDTH_IN_PIXELS / 2, LEVEL_CHUNK_HEIGHT_IN_PIXELS / 2});
	c_transform->position.x += tile_index % LEVEL_CHUNK_WIDTH * TILE_SIZE_IN_PIXELS;
	c_transform->position.y += tile_index / LEVEL_CHUNK_WIDTH * TILE_SIZE_IN_PIXELS;
	c_transform->rotation = t.rotate_ccw ? M_PI_2 : 0;

	c_sprite->size = (vec2){t.flip_x ? -atlas_entry.w : atlas_entry.w, t.flip_y ? -atlas_entry.h : atlas_entry.h};
	c_sprite->texture_x = atlas_entry.x;
	c_sprite->texture_y = atlas_entry.y;
	c_sprite->texture_w = atlas_entry.w;
	c_sprite->texture_h = atlas_entry.h;
	c_sprite->z = t.z;
	c_sprite->draw_sorted = atlas_entry.is_sorted;

	c_collider->type = atlas_entry.coll_type;
	switch (c_collider->type)
	{
		case COLLIDER_TYPE_BOX:
			c_collider->width_radius = atlas_entry.w / 2;
			c_collider->height_radius = atlas_entry.h / 2;
			break;
		default:
			break;
	}
}

void entity_tile_from_atlas(const tile_atlas_h atlas, component_sprite *c_sprite)
{
	c_sprite->texture = tile_atlas_get_texture(atlas);
}