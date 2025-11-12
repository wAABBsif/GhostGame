#include "window.h"

#include <assert.h>
#include <stddef.h>

#include "core/logging.h"
#include "camera.h"
#include "glad/glad.h"
#include "SDL3/SDL_events.h"

#define	WINDOW_MIN_WIDTH 320
#define	WINDOW_MIN_HEIGHT 240

static SDL_Window *s_window = NULL;

const char *WINDOW_TITLE = "Ghost Game";
static int s_window_width = 320;
static int s_window_height = 240;

const SDL_WindowFlags WINDOW_FLAGS =
{
	SDL_WINDOW_OPENGL
	| SDL_WINDOW_RESIZABLE
};

SDL_Window *window_create(void)
{
	log_message("Creating window...");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	s_window = SDL_CreateWindow(WINDOW_TITLE, s_window_width, s_window_height, WINDOW_FLAGS);
	assert(s_window != NULL);

	SDL_SetWindowMinimumSize(s_window, s_window_width, s_window_height);

	return s_window;
}

void window_destroy(void)
{
	SDL_DestroyWindow(s_window);
}

SDL_Window *window_get_sdl_handle(void)
{
	return s_window;
}

void window_handle_sdl_event(const SDL_Event *event)
{
	if (event->type == SDL_EVENT_KEY_DOWN)
		if (event->key.key == SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11))
			SDL_SetWindowFullscreen(s_window, SDL_GetWindowFlags(s_window) & SDL_WINDOW_FULLSCREEN ? false : true);

	if (event->type != SDL_EVENT_WINDOW_RESIZED)
		return;

	s_window_width = event->window.data1;
	s_window_height = event->window.data2;

	if (s_window_width <= 0)
		s_window_width = 1;

	if (s_window_height <= 0)
		s_window_height = 1;

	camera_window_resize(get_main_camera());

	log_message("Resizing window (%i, %i)", s_window_width, s_window_height);
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