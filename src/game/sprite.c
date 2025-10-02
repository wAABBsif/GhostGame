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

	sprite s2;
	s2.x = 120 + cosf(game_time_get_elapsed()) * 20;
	s2.y = 24;
	s2.rotation = 0;
	s2.scale_x = 0x10;
	s2.scale_y = 0x10;
	s2.color = GREEN;
	s2.z = 4;
	s2.texture = texture_get("res/sprites/test2.png");
	s2.texture_w = 16;
	s2.texture_h = 16;
	s2.texture_x = 0;
	s2.texture_y = 16;
	sprite_draw(s2, true);

	sprite s3;
	s3.x = 120 + sinf(game_time_get_elapsed()) * 20;
	s3.y = 24;
	s3.rotation = 0;
	s3.scale_x = 0x10;
	s3.scale_y = 0x10;
	s3.color = RED;
	s3.z = 3;
	s3.texture = texture_get("res/sprites/test2.png");
	s3.texture_w = 16;
	s3.texture_h = 16;
	s3.texture_x = 0;
	s3.texture_y = 16;
	sprite_draw(s3, true);

	sprite s;
	s.x = 120;
	s.y = 24;
	s.rotation = (uint16_t)(game_time_get_elapsed() * 5000);
	s.scale_x = 0x10;
	s.scale_y = 0x10;
	s.color = WHITE;
	s.z = 2;
	s.texture = texture_get("res/sprites/test.png");
	s.texture_w = 16;
	s.texture_h = 16;
	s.texture_x = 0;
	s.texture_y = 16;
	sprite_draw(s, false);
}