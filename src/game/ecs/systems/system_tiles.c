#include "system_tiles.h"

#include <assert.h>
#include <string.h>

#include "core/game_time.h"
#include "core/logging.h"
#include "game/ecs/ecs_component.h"
#include "game/ecs/ecs_entity.h"
#include "game/ecs/ecs_system.h"
#include "game/ecs/components/component_sprite.h"
#include "game/ecs/components/component_tile.h"
#include "game/ecs/components/component_transform.h"
#include "game/ecs/entities/entity_tile.h"
#include "game/level/level.h"
#include "gfx/camera.h"
#include "gfx/tiles.h"

static chunk_index s_screen_chunk_indices[4] = {-1, -1, -1, -1};

static bool s_is_marked_dirty;
static uint16_t s_tile_count_per_frame = 0;

static int8_t s_get_screen_chunk_from_position(const vec2 position)
{
	const vec2 displacement = vec2_sub(position, get_main_camera()->position);

	if (displacement.x > LEVEL_CHUNK_WIDTH_IN_PIXELS)
		return -1;

	if (displacement.x < -LEVEL_CHUNK_WIDTH_IN_PIXELS)
		return -1;

	if (displacement.y > LEVEL_CHUNK_HEIGHT_IN_PIXELS)
		return -1;

	if (displacement.y < -LEVEL_CHUNK_HEIGHT_IN_PIXELS)
		return -1;

	//normal xy quadrants

	if (displacement.x >= 0)
	{
		if (displacement.y >= 0)
			return 0;
		else
			return 3;
	}
	else
	{
		if (displacement.y >= 0)
			return 1;
		else
			return 2;
	}
}

static chunk_index s_check_chunk_index(const uint8_t index)
{
	if (s_screen_chunk_indices[index] >= 0)
	{
		const vec2 position = level_chunk_position_to_vec2(*level_get_chunk_position(s_screen_chunk_indices[index]));
		if (s_get_screen_chunk_from_position(position) == index)
			return s_screen_chunk_indices[index];
	}

	for (chunk_index i = 0; i < level_get_chunk_count(); i++)
	{
		const vec2 position = level_chunk_position_to_vec2(*level_get_chunk_position(i));
		if (s_get_screen_chunk_from_position(position) == index)
			return i;
	}

	return -1;
}

void system_tiles_load_level(void)
{

}

void system_tiles_begin_frame(void)
{
	s_is_marked_dirty = false;
	s_tile_count_per_frame = 0;

	for (int i = 0; i < 4; i++)
	{
		const chunk_index result = s_check_chunk_index(i);
		if (result == s_screen_chunk_indices[i])
			continue;

		s_is_marked_dirty = true;
		s_screen_chunk_indices[i] = result;
	}

	set_tile_origin(vec2_sub(level_chunk_position_to_vec2(*level_get_chunk_position(s_screen_chunk_indices[0])), (vec2){256, 128}));
}

void system_tiles_update(const entity_id entity)
{
	if (!s_is_marked_dirty)
		return;

	component_transform *c_transform = component_get(COMPONENT_TYPE_TRANSFORM, entity);
	component_collider *c_collider = component_get(COMPONENT_TYPE_COLLIDER, entity);
	const component_tile *c_tile = component_get(COMPONENT_TYPE_TILE, entity);

	if (s_screen_chunk_indices[c_tile->chunk_index] < 0)
		return;

	entity_tile_from_chunk(s_screen_chunk_indices[c_tile->chunk_index], s_tile_count_per_frame, c_transform, c_collider);
	entity_tile_to_quad(s_screen_chunk_indices[c_tile->chunk_index], c_tile->chunk_index, s_tile_count_per_frame, c_transform);

	s_tile_count_per_frame = (s_tile_count_per_frame + 1) % LEVEL_CHUNK_SIZE;
}
