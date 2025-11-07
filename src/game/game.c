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

	const tile_atlas_h h = tile_atlas_load("res/tiles/test_tile.atlas");
}

static void s_game_update(void)
{
	game_time_update();
	sdl_interface_update();
	input_update();

	static vec2 pos = (vec2){-256, 0};
	pos = vec2_add(pos, (vec2){game_time_get_delta() * 32, 0});
	entity_index in = system_collision_overlap_point(pos, ENTITY_INDEX_INVALID);

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