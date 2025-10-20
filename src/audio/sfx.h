#pragma once
#include "audio.h"

#define MAX_SFX_TRACKS	12

void sfx_init(void);
void sfx_update(void);
void sfx_terminate(void);

void sfx_set_global_volume(float volume);
float sfx_get_global_volume(void);

audio_track_h sfx_set(audio_clip_h clip);
void sfx_play(audio_track_h track);
void sfx_stop(audio_track_h track);
bool sfx_is_playing(audio_track_h track);

void sfx_set_volume(audio_track_h track, float volume);
float sfx_get_volume(audio_track_h track);

void sfx_set_position(audio_track_h track, vec2 position);
vec2 sfx_get_position(audio_track_h track);