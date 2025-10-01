#include "hash_map.h"

#include <stdlib.h>
#include <string.h>

#include "logging.h"

hash hash_string(const char *key)
{
	ssize_t hash = 5381;
	ssize_t c = 0;

	while ((c = *key++))
		hash = (hash << 5) + hash + c;

	return hash;
}

void hash_map_create(hash_map *map, const ssize_t entry_size, const ssize_t capacity)
{
	map->entry_size = entry_size;
	map->size = 0;
	map->capacity = capacity;
	map->entries = calloc(map->capacity, entry_size);
}

void hash_map_destroy(hash_map *map)
{
	map->entry_size = 0;
	map->size = 0;
	map->capacity = 0;
	free(map->entries);
}

ssize_t hash_map_add(hash_map *map, const void *data)
{
	if (map->size >= map->capacity)
	{
		log_error("Attempted to add new elements to hash_map at full capacity.");
		return -1;
	}

	ssize_t new_index = 0;
	hash *loc = hash_map_index(map, new_index);

	if (map->size >= 1)
	{
		new_index = hash_map_get_range(map, *(hash*)data, 0, map->size - 1);
		if (*(hash*)hash_map_index(map, new_index) == *(hash*)data)
		{
			log_error("Key already exists.");
			return -1;
		}

		loc = hash_map_index(map, new_index);
		new_index++;
		memmove(hash_map_index(map, new_index), loc, map->entry_size * (map->size - new_index + 1));
	}

	memcpy(loc, data, map->entry_size);
	map->size++;
	return new_index;
}

void hash_map_remove(hash_map *map, const ssize_t index)
{
	map->size--;
	hash *loc = hash_map_index(map, index);
	memmove(loc, hash_map_index(map, index + 1), sizeof(map->entry_size) * (map->size - index));
}

void *hash_map_index(const hash_map *map, ssize_t index)
{
	index *= map->entry_size;
	return (hash*)((char*)map->entries + index);
}

ssize_t hash_map_get_range(const hash_map *map, const hash h, const ssize_t start_index, const ssize_t end_index)
{
	if (start_index >= end_index)
		return start_index;

	const ssize_t mid = (start_index + end_index) / 2;
	const hash mid_hash = *(hash*)hash_map_index(map, mid);

	if (mid_hash < h)
		return hash_map_get_range(map, h, mid + 1, end_index);
	if (mid_hash > h)
		return hash_map_get_range(map, start_index, start_index, mid - 1);
	return mid;
}

ssize_t hash_map_get_index(const hash_map *map, const hash h)
{
	if (map->size < 1)
		return -1;

	const ssize_t index = hash_map_get_range(map, h, 0, map->size - 1);
	if (*(hash*)hash_map_index(map, index) != h)
		return -1;

	return index;
}