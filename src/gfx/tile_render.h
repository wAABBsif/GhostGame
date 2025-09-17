#pragma once
#include <stdint.h>

#include "texture.h"

//LAYOUT: XXXXXXXX XXYYYYYY YYYYZZZZ VVVVUUUU
typedef uint32_t tile_vertex;

typedef struct tile_quad
{
	tile_vertex vertices[4];
} tile_quad;

typedef struct tile tile;

void tile_rendering_init();
void tile_rendering_terminate();

void tile_rendering_set_texture(texture texture);

//x < 1024
tile_vertex tile_vertex_set_x(tile_vertex vertex, uint32_t x);
//y < 1024
tile_vertex tile_vertex_set_y(tile_vertex vertex, uint32_t y);
//z < 16
tile_vertex tile_vertex_set_z(tile_vertex vertex, uint32_t z);
//u < 16
tile_vertex tile_vertex_set_u(tile_vertex vertex, uint32_t u);
//v < 16
tile_vertex tile_vertex_set_v(tile_vertex vertex, uint32_t v);

//x < 1024 + y < 1024
tile_vertex tile_vertex_set_xy(tile_vertex vertex, uint32_t x, uint32_t y);
//uv < 256
tile_vertex tile_vertex_set_uv(tile_vertex vertex, uint32_t uv);
tile_vertex tile_vertex_set_all(tile_vertex vertex, uint32_t x, uint32_t y, uint32_t z, uint32_t uv);

uint16_t tile_vertex_get_x(tile_vertex vertex);
uint16_t tile_vertex_get_y(tile_vertex vertex);
uint8_t tile_vertex_get_z(tile_vertex vertex);
uint8_t tile_vertex_get_u(tile_vertex vertex);
uint8_t tile_vertex_get_v(tile_vertex vertex);
uint8_t tile_vertex_get_uv(tile_vertex vertex);

void tile_rendering_add_tile(const uint16_t x, const uint16_t y, const tile t);

void tile_rendering_draw(void);