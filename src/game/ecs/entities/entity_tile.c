#include "entity_tile.h"

#include "core/vec2.h"
#include "../components/component_transform.h"
#include "../components/component_sprite.h"
#include "game/level/tile.h"

entity_index entity_tile_create(const tile_atlas_h atlas, const tile tile)
{
	const atlas_tile atlas_tile = tile_atlas_get_tile(atlas, tile.tile_index);
	const entity_index result = entities_add();

	// component_transform *c_pos =  entity_add_component(COMPONENT_TYPE_TRANSFORM);
	// c_pos->position = pos;
	// c_pos->rotation = 0;
	//
	// component_sprite *c_spr =  entity_add_component(COMPONENT_TYPE_SPRITE);
	// c_spr->size = (vec2){atlas_tile.w, atlas_tile.h};
	// c_spr->texture = tile_atlas_get_texture(atlas);
	// c_spr->texture_x = atlas_tile.x;
	// c_spr->texture_y = atlas_tile.y;
	// c_spr->texture_w = atlas_tile.w;
	// c_spr->texture_h = atlas_tile.h;
	// c_spr->color = COLOR_WHITE;
	// c_spr->z = z;
	// c_spr->use_camera_to_screen_matrix = false;
	// c_spr->draw_sorted = false;
	//
	// if (atlas_tile.coll_type != COLLIDER_TYPE_NONE)
	// {
	// 	component_collider *c_coll = entity_add_component(COMPONENT_TYPE_COLLIDER);
	// 	switch (atlas_tile.coll_type)
	// 	{
	// 		case COLLIDER_TYPE_BOX:
	// 			c_coll->type = COLLIDER_TYPE_BOX;
	// 			c_coll->box.w = atlas_tile.w;
	// 			c_coll->box.h = atlas_tile.h;
	// 			break;
	// 		default:
	// 			break;
	// 	}
	// }

	return result;
}
