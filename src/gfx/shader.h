#pragma once

typedef struct shader shader;

void shader_init(void);
void shader_clear(void);

/**
 * Loads a shader from a "frag.glsl" and "vert.glsl" file
 *
 * \param name the name of the file without a file extension
 * \returns the loaded shader
 */
shader* shader_load(const char* name);

/**
 * Gets a handle to a shader using name or loads it if it doesn't exist
 *
 * \param name the name of the file without a file extension
 * \returns the requested shader
 */
shader* shader_get(const char* name);