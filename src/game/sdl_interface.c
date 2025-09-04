#include "sdl_interface.h"
#include "core/logging.h"
#include "SDL3/SDL.h"

static bool _is_ready_to_quit = false;

static void _sdl_interface_handle_events(void);

bool sdl_interface_init(void)
{
	const SDL_InitFlags flags =
	{
		SDL_INIT_EVENTS
		| SDL_INIT_VIDEO
		// | SDL_INIT_AUDIO
		// | SDL_INIT_JOYSTICK
		// | SDL_INIT_GAMEPAD
	};

	if (!SDL_Init(flags))
	{
		log_error("SDL failed to initialize (SDL Error: %s)\n", SDL_GetError());
		return false;
	}

	log_message("Initialized SDL");
	return true;
}

void sdl_interface_update(void)
{
	_sdl_interface_handle_events();
}

void sdl_interface_terminate(void)
{
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
	}
}