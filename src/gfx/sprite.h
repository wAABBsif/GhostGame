#pragma once
#include <stdbool.h>

#include "color.h"
#include "texture.h"

#define MAX_SPRITES				1024
#define MAX_SPRITE_TEXTURES		16

typedef struct sprite_vertex
{
	int16_t x;
	int16_t y;

	uint16_t texture_x;
	uint16_t texture_y;

	color    color;

	int8_t z;
	bool use_camera_to_screen_matrix;
	uint8_t texture_index;
} sprite_vertex;

typedef struct sprite_quad
{
	sprite_vertex vertices[4];
} sprite_quad;

void sprite_init(void);
void sprite_terminate(void);

bool sprite_simple_cull(vec2 position, vec2 size);
uint8_t sprite_get_texture_num(texture_h h);
void add_sprite_quad(const sprite_quad *quad, bool is_sorted);
void draw_sprites(void);