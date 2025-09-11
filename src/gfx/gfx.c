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

	int width, height;
	window_get_size(&width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_GL_SetSwapInterval(0);
	glClearColor(0.5, 0.5, 0.5, 1.0f);

	const float vertices[] =
	{
		-160,  -120,  0.0f,
		 160,  -120,  0.0f,
		 160,   120,  0.0f,
		 -160,  120,  0.0f
	};

	const unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	int vertex_buffer = 0;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);

	int index_buffer = 0;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	const shader_h s = shader_get("res/test");
	shader_set(s);
}

vec2 position = VEC2_ZERO;
float rotation = 0.0;
float size = 240;

void gfx_draw(void)
{
	int height;
	window_get_size(NULL, &height);

	bool *keys = SDL_GetKeyboardState(NULL);
	position.x += keys[SDL_SCANCODE_D] * game_time_get_delta() * 400;
	position.x -= keys[SDL_SCANCODE_A] * game_time_get_delta() * 400;
	position.y += keys[SDL_SCANCODE_W] * game_time_get_delta() * 400;
	position.y -= keys[SDL_SCANCODE_S] * game_time_get_delta() * 400;

	rotation -= keys[SDL_SCANCODE_Q] * game_time_get_delta() * 4;
	rotation += keys[SDL_SCANCODE_E] * game_time_get_delta() * 4;

	size -= keys[SDL_SCANCODE_Z] * game_time_get_delta() * 400;
	size += keys[SDL_SCANCODE_X] * game_time_get_delta() * 400;

	const camera c = {position, rotation, size};
	const mat3 m = world_to_screen_matrix(&c);
	shader_set_mat3("test", m);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	SDL_GL_SwapWindow(window_get_handle());
}

void gfx_terminate(void)
{
	log_message("Terminating graphics...");

	shader_clear();
	window_destroy();
}