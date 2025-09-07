#include "gfx.h"
#include "core/logging.h"
#include <SDL3/SDL_opengl.h>

#include "window.h"
#include "SDL3/SDL_video.h"

static SDL_GLContext s_context = NULL;

void gfx_init(void)
{
	log_message("Initializing graphics...");

	SDL_Window *window = window_create();
	if (window == NULL)
		return;

	s_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, s_context);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	SDL_GL_SetSwapInterval(0);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void gfx_draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_GL_SwapWindow(window_get_handle());
}

void gfx_terminate(void)
{
	log_message("Terminating graphics...");

	window_destroy();
}