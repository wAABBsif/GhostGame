#pragma once

union SDL_Event;
typedef union SDL_Event SDL_Event;

void debug_ui_init();
void debug_ui_terminate();
void debug_ui_update();
void debug_ui_draw();
void debug_ui_handle_sdl_event(const SDL_Event *event);