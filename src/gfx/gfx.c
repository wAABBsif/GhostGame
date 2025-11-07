#include "gfx.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "core/logging.h"
#include "glad/glad.h"

#include "shader.h"
#include "window.h"
#include "camera.h"
#include "lighting.h"
#include "sprite.h"
#include "SDL3/SDL_video.h"

static SDL_GLContext s_context = NULL;

camera cam;

void test_camera_init(void)
{
	cam = (camera){0, 0, 0, 320, 320};
	camera_create(&cam);
	set_main_camera(&cam);
}

void gfx_init(void)
{
	log_message("Initializing graphics...");

	SDL_Window *window = window_create();
	assert(window != NULL);

	s_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, s_context);
	const int glad_status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	assert(glad_status);

	shader_init();
	texture_init();
	camera_init();

	int width, height;
	window_get_size(&width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_GL_SetSwapInterval(0);

	test_camera_init();
	sprite_init();
	lighting_init();
}

void gfx_draw(void)
{
	camera_bind_main_framebuffer(&cam);
	glClearColor(0.2, 0.3, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_sprites();

	camera_bind_lighting_framebuffer(&cam);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_lighting();

	camera_unbind_framebuffer();
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	camera_render_to_screen(&cam);

	SDL_GL_SwapWindow(window_get_sdl_handle());
}

void gfx_terminate(void)
{
	log_message("Terminating graphics...");

	lighting_terminate();
	sprite_terminate();
	shader_clear();
	texture_clear();
	camera_terminate();
	window_destroy();
}