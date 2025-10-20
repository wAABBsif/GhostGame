#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "core/hash.h"

#define AUDIO_3D_SCALE		0.005f
#define MAX_AUDIO_CLIPS		64
#define MAX_AUDIO_TRACKS	16

typedef hash audio_clip_h;
typedef int16_t audio_track_h;

typedef struct vec2 vec2;

void audio_init(void);
void audio_update(void);
void audio_terminate(void);

audio_clip_h audio_clip_load(const char* name, bool is_sfx);
void audio_clip_unload(audio_clip_h h);
audio_clip_h audio_clip_get(const char* name);

audio_track_h audio_create_track(void);
void audio_destroy_track(audio_track_h track);
bool audio_track_is_valid(audio_track_h track);

void audio_set_track_clip(audio_track_h track, audio_clip_h clip);
void audio_play_track(audio_track_h track, bool loop);
void audio_pause_track(audio_track_h track);
void audio_stop_track(audio_track_h track);
bool audio_track_is_playing(audio_track_h track);

void audio_set_track_volume(audio_track_h track, float volume);
float audio_get_track_volume(audio_track_h track);

void audio_set_track_position(audio_track_h track, vec2 position);
vec2 audio_get_track_position(audio_track_h track);