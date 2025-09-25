#pragma once
#include <stdint.h>
#include "core/hash.h"

#define MAX_AUDIO_TRACKS 64

typedef hash audio_clip_h;
typedef int16_t audio_track_h;

void audio_init(void);
void audio_update(void);
void audio_terminate(void);

audio_track_h audio_create_track(void);
void audio_destroy_track(audio_track_h track);