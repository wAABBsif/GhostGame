#pragma once
#include "SDL3/SDL_video.h"

SDL_Window *window_create(void);
void window_destroy(void);
SDL_Window *window_get_handle(void);