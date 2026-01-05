#pragma once
#include <stdint.h>

#include "window.h"

typedef struct game_window game_window;

void gfx_init(void);
void gfx_draw(void);
void gfx_terminate(void);

game_window *gfx_get_window(void);

uint16_t *gfx_generate_quad_indices(const uint16_t quad_count, uint16_t *element_count);