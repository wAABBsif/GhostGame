#include "hash_map.h"

#include <stdlib.h>
#include <string.h>

#include "logging.h"

void *hash_map_index(const hash_map *map, size_t index);

hash hash_string(const char *key)
{
	size_t hash = 5381;
	size_t c = 0;

	while ((c = *key++))
		hash = (hash << 5) + hash + c;

	return hash;
}

void hash_map_create(hash_map *map, const size_t entry_size, const size_t capacity, void *entries)
{
	map->entry_size = entry_size;
	map->size = 0;
	map->capacity = capacity;
	map->entries = entries;
}

void hash_map_destroy(hash_map *map)
{
	map->entry_size = 0;
	map->size = 0;
	map->capacity = 0;
	map->entries = NULL;
}

size_t hash_map_add(hash_map *map, const void *data)
{
	if (map->size >= map->capacity)
	{
		log_error("Attempted to add new elements to hash_map at full capacity.");
		return SIZE_MAX;
	}

	size_t new_index = 0;
	hash *loc = hash_map_index(map, new_index);

	if (map->size >= 1)
	{
		new_index = hash_map_get_range(map, *(hash*)data, 0, map->size - 1);
		const hash current_index_hash = *(hash*)hash_map_index(map, new_index);

		if (current_index_hash == *(hash*)data)
		{
			log_warning("Key already exists.");
			return SIZE_MAX;
		}
		if (*(hash*)data > current_index_hash)
			new_index++;

		loc = hash_map_index(map, new_index);
		new_index++;
		memmove(hash_map_index(map, new_index), loc, map->entry_size * (map->size - (new_index - 1)));
	}

	memcpy(loc, data, map->entry_size);
	map->size++;
	return new_index;
}

void hash_map_remove(hash_map *map, const size_t index)
{
	map->size--;
	hash *loc = hash_map_index(map, index);
	memmove(loc, hash_map_index(map, index + 1), sizeof(map->entry_size) * (map->size - index));
}

void *hash_map_index(const hash_map *map, size_t index)
{
	index *= map->entry_size;
	return (hash*)((char*)map->entries + index);
}

size_t hash_map_get_range(const hash_map *map, const hash h, const size_t start_index, const size_t end_index)
{
	if (map->size <= 1)
		return 0;

	if (start_index >= end_index || end_index == SIZE_MAX)
		return start_index;

	const size_t mid = (start_index + end_index) / 2;
	const hash mid_hash = *(hash*)hash_map_index(map, mid);

	if (mid_hash < h)
		return hash_map_get_range(map, h, mid + 1, end_index);
	if (mid_hash > h)
		return hash_map_get_range(map, start_index, start_index, mid - 1);
	return mid;
}

size_t hash_map_get_index(const hash_map *map, const hash h)
{
	if (map->size < 1)
		return SIZE_MAX;

	const size_t index = hash_map_get_range(map, h, 0, map->size - 1);
	if (*(hash*)hash_map_index(map, index) != h)
		return SIZE_MAX;

	return index;
}