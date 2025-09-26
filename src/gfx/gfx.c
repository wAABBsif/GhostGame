#include "gfx.h"

#include <math.h>
#include <stdlib.h>

#include "core/logging.h"
#include "glad/glad.h"
#include <SDL3/SDL_opengl.h>

#include "shader.h"
#include "window.h"
#include "core/game_time.h"
#include "core/mat3.h"
#include "camera.h"
#include "tile_renderer.h"
#include "game/tile.h"
#include "SDL3/SDL_video.h"

static SDL_GLContext s_context = NULL;

camera cam;

void test_camera_init(void)
{
	cam = (camera){0, 0, 0, 320, 320};
	camera_create(&cam);
	set_main_camera(&cam);
}

void test_camera_update(void)
{
	const bool *keys = SDL_GetKeyboardState(NULL);
	cam.position.x += keys[SDL_SCANCODE_D] * game_time_get_delta() * 400;
	cam.position.x -= keys[SDL_SCANCODE_A] * game_time_get_delta() * 400;
	cam.position.y += keys[SDL_SCANCODE_W] * game_time_get_delta() * 400;
	cam.position.y -= keys[SDL_SCANCODE_S] * game_time_get_delta() * 400;

	cam.rotation -= keys[SDL_SCANCODE_Q] * game_time_get_delta() * 4;
	cam.rotation += keys[SDL_SCANCODE_E] * game_time_get_delta() * 4;

	cam.size -= keys[SDL_SCANCODE_Z] * game_time_get_delta() * 400;
	cam.size += keys[SDL_SCANCODE_X] * game_time_get_delta() * 400;

	vec2 min, max;
	camera_get_bounds(&cam, &min, &max);
}

void gfx_init(void)
{
	log_message("Initializing graphics...");

	SDL_Window *window = window_create();
	if (window == NULL)
		return;

	s_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, s_context);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		log_message("Failed to initialize GLAD");
		return;
	}

	shader_init();
	texture_init();
	camera_init();
	tile_renderer_init();

	int width, height;
	window_get_size(&width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_GL_SetSwapInterval(0);

	test_camera_init();
}

void gfx_draw(void)
{
	test_camera_update();

	camera_bind_framebuffer(&cam);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tile_renderer_draw();

	camera_unbind_framebuffer();
	camera_render_to_screen(&cam);

	SDL_GL_SwapWindow(window_get_sdl_handle());
}

void gfx_terminate(void)
{
	log_message("Terminating graphics...");

	shader_clear();
	texture_clear();
	camera_terminate();
	window_destroy();
	tile_renderer_terminate();
}