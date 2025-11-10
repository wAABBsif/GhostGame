#include "level.h"

#include <assert.h>
#include <stdlib.h>

#include "core/logging.h"
#include "game/ecs/entities/entity_tile.h"
#include "game/ecs/systems/system_tiles.h"
#include "SDL3/SDL_iostream.h"

static level s_level;

void levels_init()
{
	for (int i = 0; i < LEVEL_CHUNK_SIZE * 4; i++)
	{
		const uint8_t chunk = i / LEVEL_CHUNK_SIZE;
		entity_tile_create(chunk);
	}
}

void level_load(const char* filename)
{
	assert(s_level.position_data == NULL);
	assert(s_level.tile_data == NULL);
	assert(s_level.object_data == NULL);

	SDL_IOStream* stream = SDL_IOFromFile(filename, "r");
	if (stream == NULL)
		return log_error("Cannot open level %s", filename);

	void* buffer = SDL_LoadFile_IO(stream, NULL, true);
	if (buffer == NULL)
		return log_error("Cannot open level %s", filename);

	const char *file_atlas_name = (char *)(buffer);
	const uint16_t *file_chunk_count = (uint16_t *)(buffer + 0x40);
	const chunk_position *file_chunk_position = (chunk_position *)(buffer + 0x50);
	const tile_chunk *file_chunk_tile = (tile_chunk *)((void *)file_chunk_position + sizeof(chunk_position) * *file_chunk_count);
	const object_chunk *file_chunk_object = (object_chunk *)((void *)file_chunk_tile + sizeof(tile_chunk) * *file_chunk_count);

	s_level.atlas = tile_atlas_load(file_atlas_name);
	if (s_level.atlas == HASH_INVALID)
		return log_error("Cannot open atlas %s", filename);

	s_level.chunk_count = *file_chunk_count;

	s_level.position_data = malloc(sizeof(chunk_position) * s_level.chunk_count);
	memcpy(s_level.position_data, file_chunk_position, sizeof(chunk_position) * s_level.chunk_count);

	s_level.tile_data = malloc(sizeof(tile_chunk) * s_level.chunk_count);
	memcpy(s_level.tile_data, file_chunk_tile, sizeof(tile_chunk) * s_level.chunk_count);

	s_level.object_data = malloc(sizeof(object_chunk) * s_level.chunk_count);
	memcpy(s_level.object_data, file_chunk_object, sizeof(object_chunk) * s_level.chunk_count);

	SDL_free(buffer);
	system_tiles_load_level();

	log_message("Loaded level %s", filename);
}

void level_unload()
{
	s_level.atlas = HASH_INVALID;
	s_level.chunk_count = 0;
	free(s_level.position_data);
	free(s_level.tile_data);
	free(s_level.object_data);
}

tile_atlas_h level_get_tile_atlas()
{
	return s_level.atlas;
}

chunk_index level_get_chunk_count(void)
{
	return s_level.chunk_count;
}

const chunk_position *level_get_chunk_position(const chunk_index index)
{
	assert(index < level_get_chunk_count());
	return s_level.position_data + index;
}

const tile_chunk *level_get_tile_chunk(const chunk_index index)
{
	assert(index < level_get_chunk_count());
	return s_level.tile_data + index;
}

const object_chunk *level_get_object_chunk(const chunk_index index)
{
	assert(index < level_get_chunk_count());
	return s_level.object_data + index;
}

vec2 level_chunk_position_to_vec2(const chunk_position pos)
{
	return (vec2){pos.x * LEVEL_CHUNK_WIDTH_IN_PIXELS, pos.y * LEVEL_CHUNK_HEIGHT_IN_PIXELS};
}