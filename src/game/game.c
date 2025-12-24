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
#include "ecs/ecs_deletion.h"
#include "ecs/components/component_controller.h"
#include "ecs/components/component_kinematic_body.h"
#include "ecs/components/component_light.h"
#include "ecs/components/component_movement_properties.h"
#include "ecs/components/component_update.h"
#include "ecs/entities/entity_actor.h"
#include "ecs/entities/entity_tile.h"
#include "ecs/systems/system_tiles.h"
#include "gfx/camera.h"
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
	LOG_MESSAGE("Initialized successfully!\n");

	while (s_is_game_running())
	{
		s_game_update();
	}

	s_game_terminate();
	LOG_MESSAGE("Terminated successfully!");
}

static void my_cute_lil_update(entity_id entity)
{
	component_transform *transform = component_get(COMPONENT_TYPE_TRANSFORM, entity);
	component_kinematic_body *kinematics = component_get(COMPONENT_TYPE_KINEMATIC_BODY, entity);
	const component_controller *controller = component_get(COMPONENT_TYPE_CONTROLLER, entity);
	const component_movement_properties *movement_properties = component_get(COMPONENT_TYPE_MOVEMENT_PROPERTIES, entity);
	entity_actor_apply_acceleration(kinematics, movement_properties->acceleration, controller->move);
	entity_actor_apply_ground_friction(kinematics, movement_properties->ground_friction);
	entity_actor_apply_speed_cap(kinematics, movement_properties->max_speed);
	entity_actor_apply_drag(kinematics, movement_properties->drag);
}

static void s_game_init(void)
{
	log_init();
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

	entity_id plr = entities_add();

	component_transform *c_pos =  component_add(COMPONENT_TYPE_TRANSFORM, plr);
	c_pos->position = VEC2_ZERO;
	c_pos->rotation = 0;

	component_sprite *c_sprite = component_add(COMPONENT_TYPE_SPRITE, plr);
	c_sprite->size = (vec2){16, 16};
	c_sprite->texture = texture_load("res/sprites/test.png");
	c_sprite->texture_pos = (vec2u16){0, 0};
	c_sprite->texture_size = (vec2u16){16, 16};
	c_sprite->color = COLOR_WHITE;
	c_sprite->z = 2;
	c_sprite->use_camera_to_screen_matrix = false;
	c_sprite->draw_sorted = false;

	component_light *c_light = component_add(COMPONENT_TYPE_LIGHT, plr);
	c_light->size = (vec2){32, 32};
	c_light->color = COLOR_RED;
	c_light->type = LIGHT_TYPE_RADIAL;
	c_light->priority = 4;
	c_light->z = 1;
	c_light->intensity_multiplier = 1;

	component_kinematic_body *c_kinematics = component_add(COMPONENT_TYPE_KINEMATIC_BODY, plr);
	c_kinematics->velocity = VEC2_ZERO;

	component_collider *c_collider = component_add(COMPONENT_TYPE_COLLIDER, plr);
	c_collider->radius = (vec2u8){6, 6};

	component_update *c_update = component_add(COMPONENT_TYPE_UPDATE, plr);
	c_update->func = my_cute_lil_update;

	component_controller *c_controller = component_add(COMPONENT_TYPE_CONTROLLER, plr);
	c_controller->type = CONTROLLER_TYPE_PLAYER;

	component_movement_properties *c_movement = component_add(COMPONENT_TYPE_MOVEMENT_PROPERTIES, plr);
	c_movement->acceleration = 3200;
	c_movement->ground_friction = 800;
	c_movement->max_speed = 120;
	c_movement->drag = 0.2f;
}

static void s_game_update(void)
{
	game_time_update();
	sdl_interface_update();
	input_update();
	entity_deletion_update();
	system_tiles_begin_frame();
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