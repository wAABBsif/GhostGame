#include "sprite.h"

#include "gfx/sprite_renderer.h"

void sprite_draw(const sprite s)
{
	sprite_renderer_queue_sprite(s);
}
