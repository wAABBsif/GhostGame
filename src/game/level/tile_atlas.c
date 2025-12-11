#include "tile_atlas.h"

#include <assert.h>
#include <stdalign.h>
#include <stdlib.h>

#include "core/hash_map.h"
#include "core/logging.h"
#include "SDL3/SDL_iostream.h"

typedef struct tile_atlas
{
	texture_h texture;
	uint16_t tile_count;
	atlas_tile tiles[1];
} tile_atlas;

typedef struct tile_atlas_hashmap_entry
{
	tile_atlas_h key;
	tile_atlas *value;
} tile_atlas_hashmap_entry;

static hash_map s_tile_atlases;
static tile_atlas_hashmap_entry s_tile_atlas_entries[MAX_TILE_ATLASES];

void tile_atlas_init(void)
{
	LOG_MESSAGE("Initializing tile atlases...");
	hash_map_create(&s_tile_atlases, sizeof(tile_atlas_hashmap_entry), MAX_TILE_ATLASES, s_tile_atlas_entries);
}

void tile_atlas_clear(void)
{
	LOG_MESSAGE("Clearing atlases...");
	for (int i = 0; i < s_tile_atlases.size; i++)
	{
		free(s_tile_atlas_entries[i].value);
	}

	hash_map_destroy(&s_tile_atlases);
}

tile_atlas_h tile_atlas_load(const char* filename)
{
	SDL_IOStream* stream = SDL_IOFromFile(filename, "r");
	if (stream == NULL)
		return HASH_INVALID;

	void* buffer = SDL_LoadFile_IO(stream, NULL, true);
	if (buffer == NULL)
		return HASH_INVALID;

	const char *file_texture_name = (char *)(buffer);
	const uint16_t *file_tile_count = (uint16_t *)(buffer + 0x40);
	const atlas_tile *file_tiles = (atlas_tile *)(buffer + 0x50);

	const texture_h texture = texture_load(file_texture_name);
	if (texture == HASH_INVALID)
		return HASH_INVALID;

	tile_atlas_hashmap_entry entry;
	entry.key = hash_string(filename);
	entry.value = (tile_atlas*)malloc(tile_atlas_get_size(*file_tile_count));

	entry.value->texture = texture;
	entry.value->tile_count = *file_tile_count;

	memcpy(entry.value->tiles, file_tiles, entry.value->tile_count * sizeof(atlas_tile));

	SDL_free(buffer);

	LOG_MESSAGE("Loaded tile atlas %s", filename);
	const size_t add_result = hash_map_add(&s_tile_atlases, &entry);
	assert(add_result != HASH_INVALID);

	return entry.key;
}

void tile_atlas_unload(const tile_atlas_h h)
{
	const size_t index = hash_map_get_index(&s_tile_atlases, h);
	assert(index != HASH_INVALID);
	free(s_tile_atlas_entries[index].value);
	hash_map_remove(&s_tile_atlases, index);
}

tile_atlas_h tile_atlas_get(const char* name)
{
	const hash h = hash_string(name);
	assert(hash_map_get_index(&s_tile_atlases, h) != HASH_INVALID);
	return h;
}

uint32_t tile_atlas_get_size(const uint16_t tile_count)
{
	return sizeof(tile_atlas) - sizeof(atlas_tile) + sizeof(atlas_tile) * tile_count;
}

atlas_tile tile_atlas_get_tile(const tile_atlas_h atlas, const uint16_t tile)
{
	assert(tile < tile_atlas_get_tile_count(atlas));
	return s_tile_atlas_entries[hash_map_get_index(&s_tile_atlases, atlas)].value->tiles[tile];
}

texture_h tile_atlas_get_texture(const tile_atlas_h atlas)
{
	return s_tile_atlas_entries[hash_map_get_index(&s_tile_atlases, atlas)].value->texture;
}

uint16_t tile_atlas_get_tile_count(const tile_atlas_h atlas)
{
	return s_tile_atlas_entries[hash_map_get_index(&s_tile_atlases, atlas)].value->tile_count;
}