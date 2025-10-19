#pragma once
#include <stdint.h>

#include "drawing.h"
#include "game/sprite.h"

typedef struct sprite_vertex
{
	uint16_t x;
	uint16_t y;

	uint16_t texture_x;
	uint16_t texture_y;

	color    color;
	//8th bit set means a vertex uses Camera to Screen Matrix
	uint8_t  z;
	uint8_t  texture_index;
} sprite_vertex;

typedef struct sprite_quad
{
	sprite_vertex vertices[4];
} sprite_quad;

void sprite_renderer_init(void);
void sprite_renderer_terminate(void);

draw_command *create_sprite_render_command(uint16_t sprite_limit, uint16_t draw_order, command_type type);
draw_command *destroy_sprite_render_command(draw_command *cmd);

uint8_t sprite_renderer_get_texture(texture_h h);

sprite_quad sprite_renderer_create_quad(sprite s);
void sprite_renderer_add_quad_unsorted(sprite_quad quad);
void sprite_renderer_add_quad_sorted(sprite_quad quad);

void sprite_renderer_clear_frame(void);

void sprite_renderer_draw(const draw_command *cmd, const sprite_quad *quads);
void sprite_renderer_draw_unsorted(draw_command *cmd);
void sprite_renderer_draw_sorted(draw_command *cmd);