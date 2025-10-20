#include "object_test.h"

#include <math.h>

#include "core/game_time.h"
#include "core/logging.h"
#include "game/sprite.h"

void object_test_start(void *generic_object)
{
	object_test *object = (object_test *)generic_object;
	object->rotation = 0;

	log_message("Started test object!");
}

void object_test_update(void *generic_object)
{
	object_test *object = (object_test *)generic_object;
	object->rotation += game_time_get_delta();

	sprite s;
	s.x = 120;
	s.y = 24;
	s.rotation = sprite_rotation_from_float(object->rotation);
	s.scale_x = sprite_scale_from_float(1);
	s.scale_y = sprite_scale_from_float(1);
	s.color = WHITE;
	s.z = 2;
	s.texture = texture_get("res/sprites/test.png");
	s.texture_w = 16;
	s.texture_h = 16;
	s.texture_x = 0;
	s.texture_y = 16;
	sprite_draw(s, false);

	if (object->rotation >= M_PI)
		object_destroy(object->base.id);
}

void object_test_end(void *generic_object)
{
	log_message("Ended test object!");
}