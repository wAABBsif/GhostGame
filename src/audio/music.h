#pragma once
#include <stdbool.h>

#define MUSIC_BUFFER_SAMPLE_COUNT	1000000
#define MUSIC_BUFFER_COUNT	2

void music_init(void);
void music_update(void);
void music_clear(void);

bool music_play(const char* name);
void music_stop(void);
void music_pause(void);
void music_resume(void);