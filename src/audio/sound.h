#pragma once
#include <stdint.h>

#include "sound.h"
#include "core/hash.h"

#define MAX_SOUNDS	256

typedef struct vec2 vec2;
typedef struct mat3 mat3;

typedef uint32_t sound_id;
typedef hash sound_h;

void sounds_init(void);
void sounds_clear(void);

sound_h sound_load(const char* name);
void sound_unload(sound_h h);
sound_h sound_get(const char* name);