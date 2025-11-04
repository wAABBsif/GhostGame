#include "tile_atlas.h"

#include <assert.h>
#include <stdlib.h>

#include "core/hash_map.h"
#include "core/logging.h"

typedef struct tile_atlas
{
	texture_h key;
	texture_h texture;
	uint16_t count;
	atlas_tile tiles[1];
} tile_atlas;

typedef struct tile_atlas_file
{
	const char texture_name[32];
	uint16_t tile_count;
	atlas_tile tiles[1];
} tile_atlas_file;

static hash_map s_tile_atlases;
static tile_atlas *s_tile_atlas_entries[MAX_TILE_ATLASES];

void tile_atlas_init(void)
{
	log_message("Initializing tile atlases...");
	hash_map_create(&s_tile_atlases, sizeof(tile_atlas *), MAX_TILE_ATLASES, s_tile_atlas_entries);
}

void tile_atlas_clear(void)
{
	log_message("Clearing atlases...");
	for (int i = 0; i < s_tile_atlases.size; i++)
	{
		free(s_tile_atlas_entries[i]);
	}

	hash_map_destroy(&s_tile_atlases);
}

texture_h tile_atlas_load(const char* name)
{

}

void tile_atlas_unload(const tile_atlas_h h)
{
	const size_t index = hash_map_get_index(&s_tile_atlases, h);
	assert(index != SIZE_MAX);
	free(s_tile_atlas_entries[index]);
	hash_map_remove(&s_tile_atlases, index);
}

texture_h tile_atlas_get(const char* name)
{
	const hash h = hash_string(name);
	assert(hash_map_get_index(&s_tile_atlases, h) != SIZE_MAX);
	return h;
}

atlas_tile tile_atlas_get_tile(const tile_atlas_h atlas, const uint16_t tile)
{
	assert(tile < tile_atlas_get_tile_count(atlas));
	return s_tile_atlas_entries[hash_map_get_index(&s_tile_atlases, atlas)]->tiles[tile];
}

texture_h tile_atlas_get_texture(const tile_atlas_h atlas)
{
	return s_tile_atlas_entries[hash_map_get_index(&s_tile_atlases, atlas)]->texture;
}

uint16_t tile_atlas_get_tile_count(const tile_atlas_h atlas)
{
	return s_tile_atlas_entries[hash_map_get_index(&s_tile_atlases, atlas)]->count;
}