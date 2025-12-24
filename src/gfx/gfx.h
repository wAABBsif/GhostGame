#pragma once
#include "window.h"

typedef struct game_window game_window;

#define QUAD_INDICES {0, 1, 2, 2, 3, 0}

void gfx_init(void);
void gfx_draw(void);
void gfx_terminate(void);

game_window *gfx_get_window(void);