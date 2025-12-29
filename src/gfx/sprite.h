#pragma once
#include <stdbool.h>

#include "color.h"
#include "texture.h"
#include "core/vec2.h"

#define MAX_SPRITES				2048
#define MAX_SPRITE_TEXTURES		16

typedef struct sprite_vertex
{
	vec2i16 position;
	vec2u16 texture_size;

	color  color;
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
void add_sprite_quad(const sprite_quad *quad);
void draw_sprites(void);