#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "window.h"

typedef struct game_window game_window;

void gfx_init(void);
void gfx_draw(void);
void gfx_terminate(void);

game_window *gfx_get_window(void);

uint16_t gfx_get_length_of_element_array(uint16_t quad_count);
void gfx_generate_quad_elements(uint16_t *ptr, uint16_t quad_count);