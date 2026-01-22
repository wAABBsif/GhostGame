#pragma once

typedef struct game_window game_window;

game_window *window_create(void);
void window_destroy(game_window *window);

void window_get_size(const game_window *window, int *width, int *height);
float window_get_aspect_ratio(const game_window *window);

void window_handle_event(const game_window *window, const void *generic_event);
void window_make_context_current(const game_window *window);
void window_swap_buffers(const game_window *window);

void window_initialize_for_debug_ui(const game_window *window);