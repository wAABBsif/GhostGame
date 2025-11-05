#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "core/hash.h"

#define MAX_TEXTURES	256

typedef struct vec2 vec2;
typedef struct mat3 mat3;

typedef uint32_t texture_id;
typedef hash texture_h;

void texture_init(void);
void texture_clear(void);

texture_h texture_load(const char* name);
void texture_unload(texture_h h);
texture_h texture_get(const char* name);

void texture_set(const texture_id id, const int slot);

texture_id texture_get_id(texture_h t);
void texture_get_size(texture_h t, int32_t *width, int32_t *height);