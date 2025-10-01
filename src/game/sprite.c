#include "sprite.h"

#include <math.h>

#include "core/game_time.h"
#include "gfx/sprite_renderer.h"

void sprite_draw(const sprite s)
{
	sprite_renderer_add_sprite(s);
}

void sprite_update()
{
	sprite s2;
	s2.x = 120 + cosf(game_time_get_elapsed()) * 60;
	s2.y = 24;
	s2.rotation = 0;
	s2.scale_x = 0x10;
	s2.scale_y = 0x10;
	s2.color = WHITE;
	s2.z = 3;
	s2.texture = texture_get("res/sprites/test2.png");
	s2.texture_w = 16;
	s2.texture_h = 16;
	s2.texture_x = 0;
	s2.texture_y = 16;
	sprite_draw(s2);

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
	sprite_draw(s);
}