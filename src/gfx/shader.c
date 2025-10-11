#include "shader.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "core/game_time.h"
#include "core/hash_map.h"
#include "core/logging.h"
#include "core/mat3.h"
#include "core/vec2.h"
#include "glad/glad.h"
#include "SDL3/SDL_iostream.h"
#include "SDL3/SDL_opengl.h"

typedef uint32_t shader_program;

typedef struct shader
{
	shader_h key;
	shader_program program;
} shader;

static hash_map s_shaders;
static shader s_shader_entries[MAX_SHADERS];

void shader_init(void)
{
	log_message("Initializing shaders...");
	hash_map_create(&s_shaders, sizeof(shader), MAX_SHADERS, s_shader_entries);
}

void shader_clear(void)
{
	log_message("Clearing shaders...");
	for (int i = 0; i < s_shaders.size; i++)
	{
		glDeleteProgram(s_shader_entries[i].program);
	}

	hash_map_destroy(&s_shaders);
}

static char *s_shader_preprocessor(char *buffer);

static char *s_open_shader_file(const char* name, const char *extension)
{
	char filename[FILENAME_MAX];
	strcpy(filename, name);
	strcat(filename, extension);

	SDL_IOStream* stream = SDL_IOFromFile(filename, "r");
	if (stream == NULL)
		return NULL;

	char* buffer = SDL_LoadFile_IO(stream, NULL, true);
	if (buffer == NULL)
		return NULL;

	char *result = s_shader_preprocessor(buffer);
	SDL_free(buffer);
	return result;
}

static char *s_shader_preprocessor(char *buffer)
{
	while (*buffer < 0x20 && *buffer <= 0x7a)
	{
		if (*buffer == 0)
			return "";

		buffer++;
	}

	const size_t init_len = strlen(buffer);
	size_t len = init_len;

	const char *included_buffers[16];
	size_t num_include_buffers = 0;

	char *it_buffer = buffer - 1;

	while (*it_buffer != 0)
	{
	IT_BUFFER_LOOP_START:
		it_buffer++;
		if (*it_buffer == '#')
		{
			const char include_str[] = "#include";

			if (it_buffer + sizeof(include_str) > buffer + init_len)
				continue;

			for (size_t i = 0; i < sizeof(include_str) - 1; i++)
			{
				if (it_buffer[i] != include_str[i])
					goto IT_BUFFER_LOOP_START;
			}

			*it_buffer = 0;

			for (size_t i = 1; i < sizeof(include_str); i++)
			{
				it_buffer[i] = ' ';
			}

			it_buffer += sizeof(include_str);

			size_t i = 0;
			char file_name[256];

			for (i = 0; i < sizeof(file_name); i++)
			{
				if (it_buffer[i] == '\n' || it_buffer[i] == '\r')
					break;

				file_name[i] = it_buffer[i];
				it_buffer[i] = ' ';
			}
			file_name[i] = 0;

			included_buffers[num_include_buffers] = s_open_shader_file(file_name, "");
			len += strlen(included_buffers[num_include_buffers]);
			num_include_buffers++;
		}
	}

	size_t current_buffer_count = 0;
	char *result = malloc(len + 1);
	buffer--;

	for (int i = 0; i < len; i++)
	{
		buffer++;

		if (*buffer == 0)
		{
			if (current_buffer_count >= num_include_buffers)
				break;

			strcpy(&result[i], included_buffers[current_buffer_count]);
			i += strlen(included_buffers[current_buffer_count]);
			current_buffer_count++;
			result[i] = ' ';
			continue;
		}

		result[i] = *buffer;
	}

	result[len] = 0;
	return result;

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
		message[length - 1] = 0;
		log_error("%s shader error: %s", shader_type == GL_VERTEX_SHADER ? "Vertex" : "Fragment", message);
		glDeleteShader(id);
		return 0;
	}

	return id;
}

shader_program load_shader_program(const char *name, const uint32_t shader_type)
{
	char *glsl = s_open_shader_file(name, APPROPRIATE_EXTENSION(shader_type));
	if (!glsl)
	{
		log_error("Could not open %s%s", name, APPROPRIATE_EXTENSION(shader_type));
		return 0;
	}

	const shader_program shader = s_compile_shader(glsl, shader_type);
	if (!shader)
	{
		log_error("Could not compile %s.%s!", name, APPROPRIATE_EXTENSION(shader_type));
		return 0;
	}

	return shader;
}

shader_h shader_load(const char* name)
{
	const shader_program vert_shader = load_shader_program(name, GL_VERTEX_SHADER);
	const shader_program frag_shader = load_shader_program(name, GL_FRAGMENT_SHADER);

	if (!vert_shader || !frag_shader)
	{
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

	if (hash_map_add(&s_shaders, &result) == SIZE_MAX)
		return 0;

	log_message("Loaded shader %s", name);

	return result.key;
}

void shader_unload(const shader_h h)
{
	const size_t index = hash_map_get_index(&s_shaders, h);
	if (index == SIZE_MAX)
	{
		log_warning("Shader not found, so can't unload!");
		return;
	}

	const shader s = s_shader_entries[index];
	glDeleteProgram(s.program);
	hash_map_remove(&s_shaders, index);
}

shader_h shader_get(const char* name)
{
	const hash h = hash_string(name);
	const size_t index = hash_map_get_index(&s_shaders, h);

	if (index == SIZE_MAX)
	{
		log_message("Shader %s not found, so loading instead!", name);
		return shader_load(name);
	}
	return s_shader_entries[index].key;
}

void shader_set(const shader_h s)
{
	const shader sh = s_shader_entries[hash_map_get_index(&s_shaders, s)];
	glUseProgram(sh.program);
}

void shader_reset(void)
{
	glUseProgram(0);
}

void shader_set_int32_t(const shader_h s, const char *name, const int32_t value)
{
	const shader sh = s_shader_entries[hash_map_get_index(&s_shaders, s)];
	glUniform1i(glGetUniformLocation(sh.program, name), value);
}

void shader_set_uint32_t(const shader_h s, const char *name, const uint32_t value)
{
	const shader sh = s_shader_entries[hash_map_get_index(&s_shaders, s)];
	glUniform1ui(glGetUniformLocation(sh.program, name), value);
}

void shader_set_float(const shader_h s, const char *name, const float value)
{
	const shader sh = s_shader_entries[hash_map_get_index(&s_shaders, s)];
	glUniform1f(glGetUniformLocation(sh.program, name), value);
}

void shader_set_vec2(const shader_h s, const char *name, const vec2 value)
{
	const shader sh = s_shader_entries[hash_map_get_index(&s_shaders, s)];
	glUniform2f(glGetUniformLocation(sh.program, name), value.x, value.y);
}

void shader_set_mat3(const shader_h s, const char *name, const mat3 value)
{
	const shader sh = s_shader_entries[hash_map_get_index(&s_shaders, s)];
 	glUniformMatrix3fv(glGetUniformLocation(sh.program, name), 1, true, (float *)&value.m);
}