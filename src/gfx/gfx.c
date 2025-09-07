#include "gfx.h"

#include <stdlib.h>

#include "core/logging.h"
#include "glad/glad.h"
#include <SDL3/SDL_opengl.h>

#include "window.h"
#include "SDL3/SDL_video.h"

static const char* vertex =
	"#version 330 core\n"
	"\n"
	"layout(location = 0) in vec4 position;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_Position = position;\n"
	"}\n";

static const char* fragment =
	"#version 330 core\n"
	"\n"
	"layout(location = 0) out vec4 color;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	color = vec4(1, 0, 0, 1);\n"
	"}\n";

static SDL_GLContext s_context = NULL;

static uint32_t s_compile_shader(const char *source, uint32_t shader_type)
{
	uint32_t id = glCreateShader(shader_type);
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(id);

	int32_t result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = malloc(length);
		glGetShaderInfoLog(id, length, &length, message);
		log_message("Shader error: %s", message);
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static int s_create_shader(const char *vertex, const char *fragment)
{
	const uint32_t program = glCreateProgram();
	const uint32_t vs = s_compile_shader(vertex, GL_VERTEX_SHADER);
	const uint32_t fs = s_compile_shader(fragment, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
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
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f
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

	uint32_t shader = s_create_shader(vertex, fragment);
	glUseProgram(shader);
}

void gfx_draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	SDL_GL_SwapWindow(window_get_handle());
}

void gfx_terminate(void)
{
	log_message("Terminating graphics...");

	window_destroy();
}