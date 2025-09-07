#include <stdbool.h>
#include "game.h"

#include "sdl_interface.h"
#include "core/game_time.h"
#include "core/logging.h"
#include "gfx/gfx.h"

static void _game_init(void);
static void _game_update(void);
static void _game_terminate(void);
static bool _is_game_running(void);

void game_run(void)
{
	_game_init();
	log_message("Completed initialization");

	while ((_is_game_running()))
	{
		_game_update();
	}

	_game_terminate();
	log_message("Completed termination");
}

static void _game_init(void)
{
	sdl_interface_init();
	game_time_init();
	gfx_init();
}

static void _game_update(void)
{
	game_time_update();
	sdl_interface_update();
	gfx_draw();
}

static void _game_terminate(void)
{
	gfx_terminate();
	sdl_interface_terminate();
}

static bool _is_game_running(void)
{
	return !sdl_interface_ready_to_quit();
}