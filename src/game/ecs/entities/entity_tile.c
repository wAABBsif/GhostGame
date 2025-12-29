#include "entity_tile.h"

#include <math.h>

#include "core/vec2.h"
#include "../components/component_transform.h"
#include "../components/component_sprite.h"
#include "game/ecs/components/component_tile.h"
#include "game/level/tile.h"
#include "gfx/tiles.h"

entity_id entity_tile_create(const uint8_t chunk_index)
{
	const entity_id result = entities_add();

	component_transform *c_pos = component_add(COMPONENT_TYPE_TRANSFORM, result);
	c_pos->position = VEC2_ZERO;
	c_pos->rotation = 0;

	component_collider *c_coll = component_add(COMPONENT_TYPE_COLLIDER, result);
	c_coll->radius = (vec2u8){0, 0};

	component_tile *c_tile = component_add(COMPONENT_TYPE_TILE, result);
	c_tile->chunk_index = chunk_index;

	return result;
}

void entity_tile_from_chunk(const chunk_index chunk, const uint16_t tile_index, component_transform *c_transform, component_collider *c_collider)
{
	const tile t = level_get_tile_chunk(chunk)->tiles[tile_index];
	const atlas_tile atlas_entry = tile_atlas_get_tile(level_get_tile_atlas(), t.atlas_index);

	c_transform->position = level_chunk_position_to_vec2(*level_get_chunk_position(chunk));
	c_transform->position = vec2_sub(c_transform->position, (vec2){LEVEL_CHUNK_WIDTH_IN_PIXELS / 2, LEVEL_CHUNK_HEIGHT_IN_PIXELS / 2});
	c_transform->position.x += tile_index % LEVEL_CHUNK_WIDTH * TILE_SIZE_IN_PIXELS;
	c_transform->position.y += tile_index / LEVEL_CHUNK_WIDTH * TILE_SIZE_IN_PIXELS;
	c_transform->rotation = t.rotate_ccw ? M_PI_2 : 0;

	if (atlas_entry.has_collision)
	{
		c_collider->radius = (vec2u8){atlas_entry.size.x / 2, atlas_entry.size.y / 2};
	}
	else
	{
		c_collider->radius = (vec2u8){0, 0};
	}
}

void entity_tile_from_atlas(const tile_atlas_h atlas, component_sprite *c_sprite)
{
	c_sprite->texture = tile_atlas_get_texture(atlas);
}

void entity_tile_to_quad(const chunk_index chunk, const uint8_t relative_chunk, const uint16_t tile_index, component_transform *c_transform)
{
	const tile t = level_get_tile_chunk(chunk)->tiles[tile_index];
	const atlas_tile atlas_entry = tile_atlas_get_tile(level_get_tile_atlas(), t.atlas_index);
	tile_quad quad;

	quad.vertices[0].position = (vec2i8){0, 1};
	quad.vertices[1].position = (vec2i8){1, 1};
	quad.vertices[2].position = (vec2i8){1, 0};
	quad.vertices[3].position = (vec2i8){0, 0};

	const uint16_t tex_coords[8] =
	{
		atlas_entry.position.x, atlas_entry.position.y + atlas_entry.size.y,
		atlas_entry.position.x + atlas_entry.size.x, atlas_entry.position.y + atlas_entry.size.y,
		atlas_entry.position.x + atlas_entry.size.x, atlas_entry.position.y,
		atlas_entry.position.x, atlas_entry.position.y,
	};

	for (uint8_t v = 0; v < 4; v++)
	{
		if (relative_chunk == 0)
		{
			quad.vertices[v].position.x += LEVEL_CHUNK_WIDTH / 2;
			quad.vertices[v].position.y += LEVEL_CHUNK_HEIGHT / 2;
		}
		if (relative_chunk == 1)
		{
			quad.vertices[v].position.x -= LEVEL_CHUNK_WIDTH / 2;
			quad.vertices[v].position.y += LEVEL_CHUNK_HEIGHT / 2;
		}
		else if (relative_chunk == 2)
		{
			quad.vertices[v].position.x -= LEVEL_CHUNK_WIDTH / 2;
			quad.vertices[v].position.y -= LEVEL_CHUNK_HEIGHT / 2;
		}
		else if (relative_chunk == 3)
		{
			quad.vertices[v].position.x += LEVEL_CHUNK_WIDTH / 2;
			quad.vertices[v].position.y -= LEVEL_CHUNK_HEIGHT / 2;
		}

		quad.vertices[v].position.x += (int8_t)(tile_index % LEVEL_CHUNK_WIDTH - LEVEL_CHUNK_WIDTH / 2);
		quad.vertices[v].position.y += (int8_t)(tile_index / LEVEL_CHUNK_WIDTH - LEVEL_CHUNK_HEIGHT / 2);

		int32_t w, h;
		texture_get_size(tile_atlas_get_texture(level_get_tile_atlas()), &w, &h);

		quad.vertices[v].texture_coords = (vec2u16){(float)tex_coords[v * 2] / (float)w * 65535.0f, (float)tex_coords[v * 2 + 1] / (float)h * 65535.0f};

		quad.vertices[v].z = t.z;
	}

	set_tile_quad(quad, chunk * LEVEL_CHUNK_SIZE + tile_index);
}