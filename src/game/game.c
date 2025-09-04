#include <stdbool.h>
#include "game.h"
#include "core/logging.h"

static bool _is_ready_to_quit = false;

static bool _game_init(void);
static void _game_update(void);
static void _game_terminate(void);
static bool _is_game_running(void);

void game_run(void)
{
	if (!_game_init())
		return;

	log_message("Completed initialization");

	while (_is_game_running())
	{
		_game_update();
	}

	_game_terminate();
	log_message("Completed termination");
}

static bool _game_init(void)
{
	return true;
}

static void _game_update(void)
{

}

static void _game_terminate(void)
{

}

static bool _is_game_running(void)
{
	return !_is_ready_to_quit;
}