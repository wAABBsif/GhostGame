#include "sdl_interface.h"

#include <assert.h>

#include "core/logging.h"
#include "gfx/window.h"
#include "input/input.h"
#include "SDL3/SDL.h"

const SDL_InitFlags INIT_FLAGS =
{
	SDL_INIT_EVENTS
	| SDL_INIT_VIDEO
	| SDL_INIT_AUDIO
	| SDL_INIT_JOYSTICK
	| SDL_INIT_GAMEPAD
};

static bool _is_ready_to_quit = false;

static void _sdl_interface_handle_events(void);

bool sdl_interface_init(void)
{
	LOG_MESSAGE("Initializing SDL...");

	const bool successfully_init = SDL_Init(INIT_FLAGS);
	assert(successfully_init);

	return true;
}

void sdl_interface_update(void)
{
	_sdl_interface_handle_events();
}

void sdl_interface_terminate(void)
{
	LOG_MESSAGE("Terminating SDL...");

	SDL_Quit();
}

bool sdl_interface_ready_to_quit(void)
{
	return _is_ready_to_quit;
}

void _sdl_interface_handle_events(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
			_is_ready_to_quit = true;
		input_handle_sdl_event(&event);
		window_handle_event(&event);
	}
}