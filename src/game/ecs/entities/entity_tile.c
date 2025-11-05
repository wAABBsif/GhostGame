#include "entity_tile.h"

#include "core/vec2.h"
#include "../components/component_position.h"
#include "../components/component_sprite.h"

entity_index entity_tile_create(const tile_atlas_h atlas, const uint16_t tile_index, const vec2 pos, const int8_t z)
{
	const atlas_tile tile = tile_atlas_get_tile(atlas, tile_index);
	const entity_index result = entities_add();

	component_position *c_pos =  entity_add_component(COMPONENT_TYPE_POSITION);
	c_pos->value = pos;

	component_sprite *c_spr =  entity_add_component(COMPONENT_TYPE_SPRITE);
	c_spr->size = (vec2){tile.w, tile.h};
	c_spr->texture = tile_atlas_get_texture(atlas);
	c_spr->texture_x = tile.x;
	c_spr->texture_y = tile.y;
	c_spr->texture_w = tile.w;
	c_spr->texture_h = tile.h;
	c_spr->color = COLOR_WHITE;
	c_spr->z = z;
	c_spr->use_camera_to_screen_matrix = false;
	c_spr->draw_sorted = false;

	if (tile.coll_type != COLLIDER_TYPE_NONE)
	{
		component_collider *c_coll = entity_add_component(COMPONENT_TYPE_COLLIDER);
		switch (tile.coll_type)
		{
			case COLLIDER_TYPE_BOX:
				c_coll->type = COLLIDER_TYPE_BOX;
				c_coll->box.w = tile.w;
				c_coll->box.h = tile.h;
				break;
			default:
				break;
		}
	}

	return result;
}
