#pragma once
#include <stdint.h>

#include "sound.h"
#include "core/vec2.h"

#define MAX_AUDIO_SOURCES	32
#define SOURCE_ID_INVALID	(-1)

typedef int8_t source_id;

void audio_sources_init(void);
void audio_sources_terminate(void);

source_id audio_source_create(void);
void audio_source_play(source_id source);
void audio_source_stop(source_id source);

void audio_source_set_sound(source_id source, sound_h sound);
void audio_source_set_volume(source_id source, float volume);
void audio_source_set_pitch(source_id source, float pitch);
void audio_source_set_position(source_id source, vec2 position);
void audio_source_set_velocity(source_id source, vec2 velocity);