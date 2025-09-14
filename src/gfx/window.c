#include "window.h"

#include <stddef.h>

#include "core/logging.h"
#include "camera.h"
#include "glad/glad.h"
#include "SDL3/SDL_events.h"

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
	if (s_window == NULL)
	{
		log_error("Failed to create window (SDL Error: %s)\n", SDL_GetError());
	}

	return s_window;
}

void window_destroy(void)
{
	SDL_DestroyWindow(s_window);
}

SDL_Window *window_get_handle(void)
{
	return s_window;
}

void window_handle_event(const SDL_Event *event)
{
	if (event->type != SDL_EVENT_WINDOW_RESIZED)
		return;

	s_window_width = event->window.data1;
	s_window_height = event->window.data2;

	camera_create_texture(get_main_camera());
	camera_create_renderbuffer(get_main_camera());

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
	return (float)s_window_width / (float)s_window_height;
}