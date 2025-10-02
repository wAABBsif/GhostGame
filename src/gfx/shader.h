#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "core/hash.h"
#include "drawing.h"

#define MAX_SHADERS 32
#define VERT_EXTENSION ".vert.glsl"
#define FRAG_EXTENSION ".frag.glsl"

typedef struct vec2 vec2;
typedef struct mat3 mat3;

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
shader_h shader_load(const char* name);

/**
 * Unloads a shader
 *
 * \param h the hash
 */
void shader_unload(const shader_h h);

/**
 * Gets a shader using name or loads it if it doesn't exist
 *
 * \param name the name of the file without a file extension
 * \returns the requested shader
 */
shader_h shader_get(const char* name);

// Loads a shader for use in rendering operations
void shader_set(shader_h s);

// Unloads shader from being used in rendering operations
void shader_reset(void);

void shader_set_int32_t(shader_h s, const char *name, int32_t value);
void shader_set_uint32_t(shader_h s, const char *name, uint32_t value);
void shader_set_float(shader_h s, const char *name, float value);
void shader_set_vec2(shader_h s, const char* name, vec2 value);
void shader_set_mat3(shader_h s, const char *name, mat3 value);