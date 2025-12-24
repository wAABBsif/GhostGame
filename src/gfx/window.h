#pragma once

typedef struct game_window game_window;

game_window *window_create(void);
void window_destroy(void);
game_window *window_get(void);

void window_get_size(int *width, int *height);
float window_get_aspect_ratio(void);

void window_handle_event(const void *generic_event);
void window_make_context_current(void);
void window_swap_buffers(void);