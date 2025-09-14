#pragma once
#include <stdint.h>
#include "core/hash_map.h"

typedef struct vec2 vec2;
typedef struct mat3 mat3;

typedef uint32_t texture_id;

typedef struct texture
{
	hash key;
	texture_id id;
	uint32_t width;
	uint32_t height;
} texture;

// Initializes texture system for use
void texture_init(void);

// Unloads and destroys all used textures
void texture_clear(void);

/**
 * Loads a texture from an image file
 *
 * \param name the name of the file
 * \returns the loaded texture
 */
texture texture_load(const char* name);

/**
 * Unloads a texture
 *
 * \param h the hash
 */
void texture_unload(const hash h);

/**
 * Gets a texture using name or loads it if it doesn't exist
 *
 * \param name the name of the file
 * \returns the requested texture
 */
texture texture_get(const char* name);

// Loads a texture for use in rendering operations
void texture_set(texture s, int slot);