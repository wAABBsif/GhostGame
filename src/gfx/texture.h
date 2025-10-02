#pragma once
#include <stdint.h>
#include "core/hash.h"

#define MAX_TEXTURES 256

typedef struct vec2 vec2;
typedef struct mat3 mat3;

typedef uint32_t texture_id;
typedef hash texture_h;

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
texture_h texture_load(const char* name);

/**
 * Unloads a texture
 *
 * \param h the hash
 */
void texture_unload(texture_h h);

/**
 * Gets a texture using name or loads it if it doesn't exist
 *
 * \param name the name of the file
 * \returns the requested texture
 */
texture_h texture_get(const char* name);

// Loads a texture for use in rendering operations
void texture_set(texture_h t, int slot);

texture_id texture_get_id(texture_h t);
void texture_get_size(texture_h t, uint32_t *width, uint32_t *height);