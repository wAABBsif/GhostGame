#pragma once
#include <stdint.h>
#include "core/hash_map.h"

typedef uint32_t shader_program;

typedef struct shader
{
	hash key;
	shader_program program;
} shader;

/**
 * Initializes shader system for use
 */
void shader_init(void);

/**
 * Unloads and destroys all used shaders
 */
void shader_clear(void);

/**
 * Loads a shader from a "frag.glsl" and "vert.glsl" file
 *
 * \param name the name of the file without a file extension
 * \returns the loaded shader
 */
shader* shader_load(const char* name);

/**
 * Unloads a shader
 *
 * \param name the name of the file without a file extension
 */
void shader_unload(const char* name);

/**
 * Gets a handle to a shader using name or loads it if it doesn't exist
 *
 * \param name the name of the file without a file extension
 * \returns the requested shader
 */
shader* shader_get(const char* name);

/**
 * Loads a shader for use in rendering operations
 *
 * \param s the shader to be used
 */
void shader_set(shader s);


/**
 * Unloads shader from being used in rendering operations
 */
void shader_reset(void);