#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "core/hash.h"

typedef hash shader_h;

#define MAX_SHADERS	32

#define VERT_EXTENSION ".vert.glsl"
#define FRAG_EXTENSION ".frag.glsl"

#define APPROPRIATE_EXTENSION(ext) (ext == GL_VERTEX_SHADER ? VERT_EXTENSION : FRAG_EXTENSION)

typedef struct vec2 vec2;
typedef struct mat3 mat3;

void shader_init(void);
void shader_clear(void);

shader_h shader_load(const char* name);
void shader_unload(const shader_h h);
shader_h shader_get(const char* name);

void shader_set(shader_h s);
void shader_reset(void);

void shader_set_int32_t(shader_h s, const char *name, int32_t value);
void shader_set_uint32_t(shader_h s, const char *name, uint32_t value);
void shader_set_float(shader_h s, const char *name, float value);
void shader_set_vec2(shader_h s, const char* name, vec2 value);
void shader_set_vec2i(shader_h s, const char* name, int x, int y);
void shader_set_mat3(shader_h s, const char *name, mat3 value);