#include "sprite.h"

#include <math.h>

#include "core/game_time.h"
#include "gfx/sprite_renderer.h"

void sprite_draw(const sprite s, const bool is_translucent)
{
	const sprite_quad quad = sprite_renderer_create_quad(s);

	if (is_translucent)
		sprite_renderer_add_quad_sorted(quad);
	else
		sprite_renderer_add_quad_unsorted(quad);
}

void sprite_update()
{
	sprite_renderer_clear_frame();
}

uint16_t sprite_position_from_float(float value)
{
	return value;
}

uint16_t sprite_rotation_from_float(const float value)
{
	return value * (UINT16_MAX + 1) / M_PI / 2;
}

uint8_t sprite_scale_from_float(const float value)
{
	return value * 16;
}