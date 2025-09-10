#pragma once
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"

SDL_Window *window_create(void);
void window_destroy(void);
SDL_Window *window_get_handle(void);
void window_handle_event(const SDL_Event *event);
void window_get_size(int *width, int *height);
float window_get_aspect_ratio(void);