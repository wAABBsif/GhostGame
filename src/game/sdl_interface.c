#include "sdl_interface.h"
#include "core/logging.h"
#include "gfx/window.h"
#include "SDL3/SDL.h"

const SDL_InitFlags INIT_FLAGS =
{
	SDL_INIT_EVENTS
	| SDL_INIT_VIDEO
	// | SDL_INIT_AUDIO
	// | SDL_INIT_JOYSTICK
	// | SDL_INIT_GAMEPAD
};

static bool _is_ready_to_quit = false;

static void _sdl_interface_handle_events(void);

bool sdl_interface_init(void)
{
	log_message("Initializing SDL...");

	if (!SDL_Init(INIT_FLAGS))
	{
		log_error("SDL failed to initialize (SDL Error: %s)\n", SDL_GetError());
		return false;
	}

	return true;
}

void sdl_interface_update(void)
{
	_sdl_interface_handle_events();
}

void sdl_interface_terminate(void)
{
	log_message("Terminating SDL...");

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
		window_handle_event(&event);
	}
}