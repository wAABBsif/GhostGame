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
#include "game/camera.h"
#include "SDL3/SDL_video.h"

static SDL_GLContext s_context = NULL;

camera cam;

uint32_t vao;
uint32_t vbo;
uint32_t ibo;

void test_init(void)
{
	cam = (camera){0, 0, 0, 320, 320};
	camera_create(&cam);
	set_main_camera(&cam);

	const float vertices[] =
	{
		-160,  -120,  0.0f,
		 160,  -120,  0.0f,
		 160,   120,  0.0f,
		 -160,  120,  0.0f
	};

	const unsigned int indices[] = QUAD_INDICES;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void test_update(void)
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
	camera_init();

	int width, height;
	window_get_size(&width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_GL_SetSwapInterval(0);

	test_init();
}

void gfx_draw(void)
{
	test_update();

	camera_bind_framebuffer(&cam);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vao);

	const shader_h s = shader_get("res/test");
	shader_set(s);

	const mat3 m = world_to_screen_matrix(&cam);
	shader_set_mat3(s, "test", m);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	camera_unbind_framebuffer();
	camera_render_to_screen(&cam);

	SDL_GL_SwapWindow(window_get_handle());
}

void gfx_terminate(void)
{
	log_message("Terminating graphics...");

	shader_clear();
	camera_terminate();
	window_destroy();
}