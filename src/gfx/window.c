#include "window.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "core/logging.h"
#include "camera.h"
#include "glad/glad.h"
#include "SDL3/SDL_events.h"

struct game_window
{
	SDL_Window *sdl_window;
	SDL_GLContext sdl_context;
};

#define	WINDOW_MIN_WIDTH 320
#define	WINDOW_MIN_HEIGHT 240

const char *WINDOW_TITLE = "Ghost Game";

const SDL_WindowFlags WINDOW_FLAGS =
{
	SDL_WINDOW_OPENGL
	| SDL_WINDOW_RESIZABLE
};

game_window *window_create(void)
{
	game_window *window = malloc(sizeof(game_window));
	LOG_MESSAGE("Creating window...");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window->sdl_window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT, WINDOW_FLAGS);
	if (window->sdl_window == NULL)
	{
		free(window);
		return NULL;
	}

	SDL_SetWindowMinimumSize(window->sdl_window, WINDOW_MIN_HEIGHT, WINDOW_MIN_HEIGHT);

	window->sdl_context = SDL_GL_CreateContext(window->sdl_window);

	return window;
}

void window_destroy(game_window *window)
{
	SDL_DestroyWindow(window->sdl_window);
	free(window);
}

void window_get_size(const game_window *window, int *width, int *height)
{
	assert(window != NULL);
	assert(width != NULL);
	assert(height != NULL);

	SDL_GetWindowSize(window->sdl_window, width, height);
}

float window_get_aspect_ratio(const game_window *window)
{
	int width, height;
	window_get_size(window, &width, &height);
	return (float)width / (float)height;
}

void window_handle_event(const game_window *window, const void *generic_event)
{
	const SDL_Event *event = (SDL_Event *)generic_event;
	if (event->type == SDL_EVENT_KEY_DOWN)
		if (event->key.key == SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11))
			SDL_SetWindowFullscreen(window->sdl_window, SDL_GetWindowFlags(window->sdl_window) & SDL_WINDOW_FULLSCREEN ? false : true);

	if (event->type != SDL_EVENT_WINDOW_RESIZED)
		return;

	int width = event->window.data1;
	int height = event->window.data2;

	if (width <= 0)
		width = 1;

	if (height <= 0)
		height = 1;

	SDL_SetWindowSize(window->sdl_window, width, height);

	camera_window_resize(get_main_camera());

	LOG_MESSAGE("Resizing window (%i, %i)", width, height);
}

void window_make_context_current(const game_window *window)
{
	SDL_GL_MakeCurrent(window->sdl_window, window->sdl_context);
}

void window_swap_buffers(const game_window *window)
{
	SDL_GL_SwapWindow(window->sdl_window);
}