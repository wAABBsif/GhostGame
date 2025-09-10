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

const int MAX_SHADERS = 64;

static shader_h s_active_shader;
static hash_map s_shaders;

void shader_init(void)
{
	log_message("Initializing shaders...");
	hash_map_create(&s_shaders, sizeof(struct shader), MAX_SHADERS);
	s_active_shader = NULL;
}

void shader_clear(void)
{
	log_message("Clearing shaders...");
	for (int i = 0; i < s_shaders.size; i++)
	{
		const struct shader *s = (struct shader *)hash_map_index(&s_shaders, i);
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

static shader_program s_compile_shader(const char *source, uint32_t shader_type)
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
	char *vert_glsl = s_open_shader_file(name, VERT_EXTENSION);
	char *frag_glsl = s_open_shader_file(name, FRAG_EXTENSION);

	if (!vert_glsl || !frag_glsl)
	{
		log_error("Could not open %s shader file!", name);
		if (frag_glsl)
			free(frag_glsl);
		if (vert_glsl)
			free(vert_glsl);
		return NULL;
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
		return NULL;
	}

	const struct shader result = (struct shader){hash_string(name), glCreateProgram()};

	glAttachShader(result.program, vert_shader);
	glAttachShader(result.program, frag_shader);
	glLinkProgram(result.program);
	glValidateProgram(result.program);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	return hash_map_add(&s_shaders, &result);
}

void shader_unload(const char* name)
{
	const size_t index = hash_map_get(&s_shaders, hash_string(name));
	if (index == -1)
	{
		log_warning("Shader %s not found, so can't unload!", name);
		return;
	}

	const shader_h s = hash_map_index(&s_shaders, index);
	glDeleteProgram(s->program);
	hash_map_remove(&s_shaders, index);
}

shader_h shader_get(const char* name)
{
	const size_t index = hash_map_get(&s_shaders, hash_string(name));
	if (index == -1)
	{
		log_warning("Shader %s not found, so loading instead!", name);
		return shader_load(name);
	}

	return hash_map_index(&s_shaders, index);
}

void shader_set(const shader_h s)
{
	s_active_shader = s;
	glUseProgram(s->program);
}

void shader_reset(void)
{
	s_active_shader = NULL;
	glUseProgram(0);
}

void shader_set_int32_t(const char *name, const int32_t value)
{
	glUniform1i(glGetUniformLocation(s_active_shader->program, name), value);
}

void shader_set_uint32_t(const char *name, const uint32_t value)
{
	glUniform1ui(glGetUniformLocation(s_active_shader->program, name), value);
}

void shader_set_float(const char *name, const float value)
{
	glUniform1f(glGetUniformLocation(s_active_shader->program, name), value);
}

void shader_set_vec2(const char* name, const vec2 value)
{
	glUniform2f(glGetUniformLocation(s_active_shader->program, name), value.x, value.y);
}

void shader_set_mat3(const char *name, const mat3 value)
{
 	glUniformMatrix3fv(glGetUniformLocation(s_active_shader->program, name), 1, true, &value.m);
}