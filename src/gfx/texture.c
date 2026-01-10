#include "texture.h"

#include <assert.h>

#include "core/hash_map.h"
#include "core/logging.h"
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct texture
{
	texture_h key;
	texture_id id;
	int32_t width;
	int32_t height;
} texture;

static hash_map s_textures;
static texture s_texture_entries[MAX_TEXTURES];

void texture_init(void)
{
	LOG_MESSAGE("Initializing textures...");
	hash_map_create(&s_textures, sizeof(texture), MAX_TEXTURES, s_texture_entries);
}

void texture_clear(void)
{
	LOG_MESSAGE("Clearing textures...");
	for (int i = 0; i < s_textures.size; i++)
	{
		glDeleteTextures(1, &s_texture_entries[i].id);
	}

	hash_map_destroy(&s_textures);
}

texture_h texture_load(const char* name)
{
	if (s_textures.size >= MAX_TEXTURES)
	{
		LOG_WARNING("Attempted to add texture when there are already too many textures!");
		return 0;
	}

	texture t;
	t.key = hash_string(name);
	const stbi_uc *img = stbi_load(name, &t.width, &t.height, NULL, 4);
	if (img == NULL)
	{
		LOG_ERROR("Failed to load texture from file \"%s\"", name);
		return 0;
	}

	if (t.width == 0 || t.height == 0)
	{
		LOG_ERROR("0 is not a valid width or height!");
		return 0;
	}

	glGenTextures(1, &t.id);
	glBindTexture(GL_TEXTURE_2D, t.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t.width, t.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	LOG_MESSAGE("Loaded texture %s", name);
	const size_t add_result = hash_map_add(&s_textures, &t);
	assert(add_result != HASH_INVALID);

	return t.key;
}

void texture_unload(const texture_h h)
{
	const size_t index = hash_map_get_index(&s_textures, h);
	assert(index != HASH_INVALID);
	const texture t = s_texture_entries[index];
	glDeleteTextures(1, &t.id);
	hash_map_remove(&s_textures, index);
}

texture_h texture_get(const char* name)
{
	const hash h = hash_string(name);
	assert(hash_map_get_index(&s_textures, h) != HASH_INVALID);
	return h;
}

void texture_set(const texture_id id, const int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

texture_id texture_get_id(const texture_h t)
{
	const texture tex = s_texture_entries[hash_map_get_index(&s_textures, t)];
	return tex.id;
}

void texture_get_size(const texture_h t, int32_t *width, int32_t *height)
{
	const texture tex = s_texture_entries[hash_map_get_index(&s_textures, t)];
	*width = tex.width;
	*height = tex.height;
}