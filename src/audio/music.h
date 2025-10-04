#pragma once
#include "audio.h"

void music_init(void);
void music_update(void);
void music_terminate(void);

void music_set_global_volume(float volume);
float music_get_global_volume(void);

void music_set(audio_clip_h clip);
void music_play(bool loop);
void music_pause(void);
void music_stop(void);

void music_set_volume(float volume);
float music_get_volume(void);