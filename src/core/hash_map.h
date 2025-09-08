#pragma once
#include <stdint.h>

typedef size_t hash;

typedef struct hash_map
{
	size_t entry_size;
	size_t size;
	size_t capacity;
	void *entries;
} hash_map;

/**
 * Turns a string key into a hash
 *
 * \param key the key intended to be hashed
 * \returns the resulting hash
 */
hash hash_string(const char *key);

/**
 * Initializes a hashmap
 *
 * \param map the location in which the hash map is stored
 * \param entry_size the size (in bytes) of each hash map entry, including the hash
 * \param capacity the maximum amount of entries the hash map can contain
 */
void hash_map_create(hash_map *map, size_t entry_size, size_t capacity);

/**
 * Deinitializes a hashmap
 *
 * \param map the hash map to deinitialize
 */
void hash_map_destroy(hash_map *map);

/**
 * Adds an element to a hash map
 *
 * \param map the hash map
 * \param data a pointer to the element that should be added
 * \returns a pointer to the element stored in the hashmap
 */
void *hash_map_add(hash_map *map, const void *data);

/**
 * Removes an element from a hash map
 *
 * \param map the hash map
 * \param index the index of the element
 */
void hash_map_remove(hash_map *map, size_t index);

/**
 * Retrieves a handle to a hash map element by index
 *
 * \param map the hash map
 * \param index the index of the element
 * \returns a pointer to the element
 */
void *hash_map_index(const hash_map *map, size_t index);

/**
 * Binary searches for the index of a hash map element between two indices
 *
 * \param map the hash map
 * \param h the hash of the element
 * \param start_index the first index searched through
 * \param end_index the last index searched through
 * \returns either the index of the element if it exists; otherwise, the element that would be below it.
 */
size_t hash_map_get_range(const hash_map *map, hash h, size_t start_index, size_t end_index);

/**
 * Binary searches for the index of a hash map element
 *
 * \param map the hash map
 * \param h the hash of the element
 * \returns either the index of the element if it exists; otherwise, -1
 */
size_t hash_map_get(const hash_map *map, hash h);
