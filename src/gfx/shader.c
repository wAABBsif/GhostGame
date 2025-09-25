#include "shader.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/hash_map.h"
#include "core/logging.h"
#include "core/mat3.h"
#include "core/vec2.h"
#include "glad/glad.h"
#include "SDL3/SDL_opengl.h"

const char* VERT_EXTENSION = ".vert.glsl";
const char* FRAG_EXTENSION = ".frag.glsl";

const int MAX_SHADERS = 32;

static hash_map s_shaders;

typedef uint32_t shader_program;

typedef struct shader
{
	hash key;
	shader_program program;
} shader;

void shader_init(void)
{
	log_message("Initializing shaders...");
	hash_map_create(&s_shaders, sizeof(shader), MAX_SHADERS);
}

void shader_clear(void)
{
	log_message("Clearing shaders...");
	for (int i = 0; i < s_shaders.size; i++)
	{
		const shader *s = (shader *)hash_map_index(&s_shaders, i);
		glDeleteProgram(s->program);
	}

	hash_map_destroy(&s_shaders);
}

static char *s_open_shader_file(const char* name, const char *extension)
{
	char filename[FILENAME_MAX];
	strcpy(filename, name);
	strcat(filename, extension);

	FILE* file = fopen(filename, "rb");
	if (file == NULL)
		return NULL;

	fseek(file, 0, SEEK_END);
	const int32_t size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = malloc(size + 1);
	fread(buffer, 1, size, file);
	buffer[size] = 0;

	return buffer;
}

static shader_program s_compile_shader(const char *source, const uint32_t shader_type)
{
	const uint32_t id = glCreateShader(shader_type);
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

shader_h shader_load(const char* name)
{
	if (s_shaders.size >= MAX_SHADERS)
	{
		log_warning("Attempted to add shader when there are already too many shaders!");
		return 0;
	}

	char *vert_glsl = s_open_shader_file(name, VERT_EXTENSION);
	char *frag_glsl = s_open_shader_file(name, FRAG_EXTENSION);

	if (!vert_glsl || !frag_glsl)
	{
		log_error("Could not open %s shader file!", name);
		if (frag_glsl)
			free(frag_glsl);
		if (vert_glsl)
			free(vert_glsl);
		return 0;
	}

	const shader_program vert_shader = s_compile_shader(vert_glsl, GL_VERTEX_SHADER);
	const shader_program frag_shader = s_compile_shader(frag_glsl, GL_FRAGMENT_SHADER);

	free(vert_glsl);
	free(frag_glsl);

	if (!frag_shader || !vert_shader)
	{
		log_error("Could not compile %s shader!", name);
		if (vert_shader)
			glDeleteShader(vert_shader);
		if (frag_shader)
			glDeleteShader(frag_shader);
		return 0;
	}

	const shader result = (shader){hash_string(name), glCreateProgram()};

	glAttachShader(result.program, vert_shader);
	glAttachShader(result.program, frag_shader);
	glLinkProgram(result.program);
	glValidateProgram(result.program);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	if (hash_map_add(&s_shaders, &result) < 0)
		return 0;

	log_message("Loaded shader %s", name);

	return result.key;
}

void shader_unload(const hash h)
{
	const size_t index = hash_map_get(&s_shaders, h);
	if (index == -1)
	{
		log_warning("Shader not found, so can't unload!");
		return;
	}

	const shader s = *(shader*)hash_map_index(&s_shaders, index);
	glDeleteProgram(s.program);
	hash_map_remove(&s_shaders, index);
}

shader_h shader_get(const char* name)
{
	const hash h = hash_string(name);
	const ssize_t index = hash_map_get(&s_shaders, h);

	if (index < 0)
	{
		log_warning("Shader %s not found, so loading instead!", name);
		return shader_load(name);
	}
	return ((shader *)hash_map_get(&s_shaders, h))->key;
}

void shader_set(const shader_h s)
{
	const shader *sh = (shader *)hash_map_index(&s_shaders, hash_map_get(&s_shaders, s));
	glUseProgram(sh->program);
}

void shader_reset(void)
{
	glUseProgram(0);
}

void shader_set_int32_t(const shader_h s, const char *name, const int32_t value)
{
	const shader *sh = (shader *)hash_map_index(&s_shaders, hash_map_get(&s_shaders, s));
	glUniform1i(glGetUniformLocation(sh->program, name), value);
}

void shader_set_uint32_t(const shader_h s, const char *name, const uint32_t value)
{
	const shader *sh = (shader *)hash_map_index(&s_shaders, hash_map_get(&s_shaders, s));
	glUniform1ui(glGetUniformLocation(sh->program, name), value);
}

void shader_set_float(const shader_h s, const char *name, const float value)
{
	const shader *sh = (shader *)hash_map_index(&s_shaders, hash_map_get(&s_shaders, s));
	glUniform1f(glGetUniformLocation(sh->program, name), value);
}

void shader_set_vec2(const shader_h s, const char *name, const vec2 value)
{
	const shader *sh = (shader *)hash_map_index(&s_shaders, hash_map_get(&s_shaders, s));
	glUniform2f(glGetUniformLocation(sh->program, name), value.x, value.y);
}

void shader_set_mat3(const shader_h s, const char *name, const mat3 value)
{
	const shader *sh = (shader *)hash_map_index(&s_shaders, hash_map_get(&s_shaders, s));
 	glUniformMatrix3fv(glGetUniformLocation(sh->program, name), 1, true, (float *)&value.m);
}