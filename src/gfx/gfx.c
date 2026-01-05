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
#include "tiles.h"
#include "SDL3/SDL_video.h"

static game_window *s_window;
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
	LOG_MESSAGE("Initializing graphics...");

	s_window = window_create();
	assert(s_window != NULL);
	window_make_context_current(s_window);

	const int glad_status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	assert(glad_status);

	shader_init();
	texture_init();
	camera_init();

	int width, height;
	window_get_size(s_window, &width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_GL_SetSwapInterval(0);

	test_camera_init();
	tiles_init();
	sprite_init();
	lighting_init();
}

void gfx_draw(void)
{
	camera_bind_main_framebuffer(&cam);
	glClearColor(0.2, 0.3, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_tiles();
	draw_sprites();

	camera_bind_lighting_framebuffer(&cam);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_lighting();

	camera_unbind_framebuffer();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	camera_render_to_screen(&cam);

	window_swap_buffers(s_window);
}

void gfx_terminate(void)
{
	LOG_MESSAGE("Terminating graphics...");

	lighting_terminate();
	sprite_terminate();
	tiles_terminate();
	shader_clear();
	texture_clear();
	camera_terminate();
	window_destroy(s_window);
}

game_window *gfx_get_window(void)
{
	return s_window;
}

uint16_t *gfx_generate_quad_indices(const uint16_t quad_count, uint16_t *element_count)
{
	const uint16_t indices_per_quad[6] = {0, 1, 2, 2, 3, 0};
	uint16_t *result = malloc(sizeof(indices_per_quad) * quad_count);

	for (int i = 0; i < 6 * quad_count; i++)
	{
		const int index = i % 6;
		const int quad = i / 6;
		result[i] = indices_per_quad[index] + 4 * quad;
	}

	*element_count = 6 * quad_count;
	return result;
}