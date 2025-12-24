#include "window.h"

#include <assert.h>
#include <stddef.h>

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

static game_window s_window;

const char *WINDOW_TITLE = "Ghost Game";
static int s_window_width = WINDOW_MIN_WIDTH;
static int s_window_height = WINDOW_MIN_HEIGHT;

const SDL_WindowFlags WINDOW_FLAGS =
{
	SDL_WINDOW_OPENGL
	| SDL_WINDOW_RESIZABLE
};

game_window *window_create(void)
{
	LOG_MESSAGE("Creating window...");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	s_window.sdl_window = SDL_CreateWindow(WINDOW_TITLE, s_window_width, s_window_height, WINDOW_FLAGS);
	assert(s_window != NULL);

	SDL_SetWindowMinimumSize(s_window.sdl_window, s_window_width, s_window_height);

	s_window.sdl_context = SDL_GL_CreateContext(s_window.sdl_window);

	return &s_window;
}

void window_destroy(void)
{
	SDL_DestroyWindow(s_window.sdl_window);
}

game_window *window_get(void)
{
	return &s_window;
}

void window_get_size(int *width, int *height)
{
	if (width != NULL)
		*width = s_window_width;
	if (height != NULL)
		*height = s_window_height;
}

float window_get_aspect_ratio(void)
{
	assert(s_window_height != 0);
	return (float)s_window_width / (float)s_window_height;
}

void window_handle_event(const void *generic_event)
{
	const SDL_Event *event = (SDL_Event *)generic_event;
	if (event->type == SDL_EVENT_KEY_DOWN)
		if (event->key.key == SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11))
			SDL_SetWindowFullscreen(s_window.sdl_window, SDL_GetWindowFlags(s_window.sdl_window) & SDL_WINDOW_FULLSCREEN ? false : true);

	if (event->type != SDL_EVENT_WINDOW_RESIZED)
		return;

	s_window_width = event->window.data1;
	s_window_height = event->window.data2;

	if (s_window_width <= 0)
		s_window_width = 1;

	if (s_window_height <= 0)
		s_window_height = 1;

	camera_window_resize(get_main_camera());

	LOG_MESSAGE("Resizing window (%i, %i)", s_window_width, s_window_height);
}

void window_make_context_current(void)
{
	SDL_GL_MakeCurrent(s_window.sdl_window, s_window.sdl_context);
}

void window_swap_buffers(void)
{
	SDL_GL_SwapWindow(s_window.sdl_window);
}