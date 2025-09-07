#include "window.h"

#include <stddef.h>

#include "core/logging.h"

static SDL_Window *s_window = NULL;

const char *WINDOW_TITLE = "Ghost Game";
const int WINDOW_WIDTH = 320;
const int WINDOW_HEIGHT = 240;
const SDL_WindowFlags WINDOW_FLAGS =
{
	SDL_WINDOW_OPENGL
	| SDL_WINDOW_RESIZABLE
};

SDL_Window *window_create(void)
{
	log_message("Creating window...");
	s_window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
	if (s_window == NULL)
	{
		log_error("Failed to create window (SDL Error: %s)\n", SDL_GetError());
	}
	return s_window;
}

void window_destroy(void)
{

}

SDL_Window *window_get_handle(void)
{
	return s_window;
}