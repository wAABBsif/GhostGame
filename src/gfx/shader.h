#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "core/hash_map.h"

typedef struct vec2 vec2;
typedef struct mat3 mat3;

typedef uint32_t shader_program;

typedef struct shader
{
	hash key;
	shader_program program;
} shader;

// Initializes shader system for use
void shader_init(void);

// Unloads and destroys all used shaders
void shader_clear(void);

/**
 * Loads a shader from a "frag.glsl" and "vert.glsl" file
 *
 * \param name the name of the file without a file extension
 * \returns the loaded shader
 */
shader shader_load(const char* name);

/**
 * Unloads a shader
 *
 * \param h the hash
 */
void shader_unload(const hash h);

/**
 * Gets a shader using name or loads it if it doesn't exist
 *
 * \param name the name of the file without a file extension
 * \returns the requested shader
 */
shader shader_get(const char* name);

// Loads a shader for use in rendering operations
void shader_set(shader s);

// Unloads shader from being used in rendering operations
void shader_reset(void);

void shader_set_int32_t(shader shader, const char *name, int32_t value);
void shader_set_uint32_t(shader shader, const char *name, uint32_t value);
void shader_set_float(shader shader, const char *name, float value);
void shader_set_vec2(shader shader, const char* name, vec2 value);
void shader_set_mat3(shader shader, const char *name, mat3 value);