#pragma once
#include "hash.h"

typedef struct hash_map
{
	size_t entry_size;
	size_t size;
	size_t capacity;
	void *entries;
} hash_map;

hash hash_string(const char *key);

void hash_map_create(hash_map *map, size_t entry_size, size_t capacity, void *entries);

void hash_map_destroy(hash_map *map);


size_t hash_map_add(hash_map *map, const void *data);
void hash_map_remove(hash_map *map, size_t index);

size_t hash_map_get_range(const hash_map *map, hash h, size_t start_index, size_t end_index);
size_t hash_map_get_index(const hash_map *map, hash h);
