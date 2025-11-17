#include <stdbool.h>
#include "game.h"

#include "ecs/ecs_component.h"
#include "ecs/ecs_entity.h"
#include "sdl_interface.h"
#include "ecs/ecs_system.h"
#include "audio/audio.h"
#include "core/game_time.h"
#include "core/logging.h"
#include "core/vec2.h"
#include "ecs/components/component_dynamic_body.h"
#include "ecs/components/component_kinematic_body.h"
#include "ecs/components/component_light.h"
#include "ecs/components/component_update.h"
#include "ecs/entities/entity_tile.h"
#include "ecs/systems/system_collision.h"
#include "gfx/gfx.h"
#include "input/input.h"
#include "level/tile_atlas.h"

static void s_game_init(void);
static void s_game_update(void);
static void s_game_terminate(void);
static bool s_is_game_running(void);

void game_run(void)
{
	s_game_init();
	log_message("Completed initialization");

	while (s_is_game_running())
	{
		s_game_update();
	}

	s_game_terminate();
	log_message("Completed termination");
}

static void my_cute_lil_update(const component_index *components, entity_index index)
{
	component_kinematic_body *kinematics = components_get_index(COMPONENT_TYPE_KINEMATIC_BODY, components[COMPONENT_TYPE_KINEMATIC_BODY]);
	vec2 input = input_get_vector(INPUT_ACTION_MOVE_L, INPUT_ACTION_MOVE_R, INPUT_ACTION_MOVE_D, INPUT_ACTION_MOVE_U);
	kinematics->velocity = vec2_mul(input, 7200);
}

static void s_game_init(void)
{
	sdl_interface_init();
	game_time_init();
	input_init();
	gfx_init();
	audio_init();
	components_init();
	systems_init();
	tile_atlas_init();
	levels_init();

	level_load("res/levels/test.level");

	entities_add();

	component_transform *c_pos =  entity_add_component(COMPONENT_TYPE_TRANSFORM);
	c_pos->position = VEC2_ZERO;
	c_pos->rotation = 0;

	component_sprite *c_sprite = entity_add_component(COMPONENT_TYPE_SPRITE);
	c_sprite->size = (vec2){16, 16};
	c_sprite->texture = texture_load("res/sprites/test.png");
	c_sprite->texture_x = 0;
	c_sprite->texture_y = 0;
	c_sprite->texture_w = 16;
	c_sprite->texture_h = 16;
	c_sprite->color = COLOR_WHITE;
	c_sprite->z = 2;
	c_sprite->use_camera_to_screen_matrix = false;
	c_sprite->draw_sorted = false;

	component_light *c_light = entity_add_component(COMPONENT_TYPE_LIGHT);
	c_light->size = (vec2){32, 32};
	c_light->color = COLOR_RED;
	c_light->type = LIGHT_TYPE_RADIAL;
	c_light->priority = 4;
	c_light->z = 1;

	component_kinematic_body *c_kinematics = entity_add_component(COMPONENT_TYPE_KINEMATIC_BODY);
	c_kinematics->velocity = VEC2_ZERO;

	component_dynamic_body *c_dynamics = entity_add_component(COMPONENT_TYPE_DYNAMIC_BODY);
	c_dynamics->width = 12;
	c_dynamics->height = 12;

	component_update *c_update = entity_add_component(COMPONENT_TYPE_UPDATE);
	c_update->func = my_cute_lil_update;
}

static void s_game_update(void)
{
	game_time_update();
	sdl_interface_update();
	input_update();
	systems_update();
	audio_update();
	gfx_draw();
}

static void s_game_terminate(void)
{
	tile_atlas_clear();
	components_terminate();
	input_terminate();
	audio_terminate();
	gfx_terminate();
	sdl_interface_terminate();
}

static bool s_is_game_running(void)
{
	return !sdl_interface_ready_to_quit();
}