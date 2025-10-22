#include "gfx.h"

#include <assert.h>
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
#include "sprite_renderer.h"
#include "tile_renderer.h"
#include "game/tile.h"
#include "input/input.h"
#include "SDL3/SDL_video.h"

static SDL_GLContext s_context = NULL;
static rumble_id s_rumble_id;

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
	const vec2 move_input = input_get_vector(INPUT_ACTION_MOVE_L, INPUT_ACTION_MOVE_R, INPUT_ACTION_MOVE_D, INPUT_ACTION_MOVE_U);
	cam.position = vec2_add(cam.position, vec2_mul(move_input, 400 * game_time_get_delta()));

	if (input_is_action_down(INPUT_ACTION_SHOOT))
	{
		cam.rotation -= game_time_get_delta() * 4;

		if (!input_is_rumble_active(s_rumble_id))
			s_rumble_id = input_rumble(0, 0.8, 0);
	}

	if (input_is_action_down(INPUT_ACTION_SWITCH))
		cam.rotation += game_time_get_delta() * 4;

	cam.size -= keys[SDL_SCANCODE_Z] * game_time_get_delta() * 400;
	cam.size += keys[SDL_SCANCODE_X] * game_time_get_delta() * 400;

	vec2 min, max;
	camera_get_bounds(&cam, &min, &max);
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
	sprite_renderer_init();
	tile_renderer_init();

	int width, height;
	window_get_size(&width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

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

	handle_draw_commands();

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
	sprite_renderer_terminate();
	tile_renderer_terminate();
}