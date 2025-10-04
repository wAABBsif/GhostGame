#include "texture.h"

#include "core/hash_map.h"
#include "core/logging.h"
#include "glad/glad.h"
#include "SDL3_image/SDL_image.h"

typedef struct texture
{
	texture_h key;
	texture_id id;
	int32_t width;
	int32_t height;
} texture;

static hash s_active_texture;
static hash_map s_textures;
static texture s_texture_entries[MAX_TEXTURES];

void texture_init(void)
{
	log_message("Initializing textures...");
	hash_map_create(&s_textures, sizeof(texture), MAX_TEXTURES, s_texture_entries);
	s_active_texture = 0;
}

void texture_clear(void)
{
	log_message("Clearing textures...");
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
		log_warning("Attempted to add texture when there are already too many textures!");
		return 0;
	}

	SDL_Surface *raw_img = IMG_Load(name);
	if (raw_img == NULL)
	{
		log_error("Failed to load texture from file \"%s\"", name);
		return 0;
	}

	const SDL_Surface *img = SDL_ConvertSurface(raw_img, SDL_PIXELFORMAT_RGBA4444);
	SDL_DestroySurface(raw_img);

	texture t;
	t.key = hash_string(name);
	t.width = img->w;
	t.height = img->h;

	glGenTextures(1, &t.id);
	glBindTexture(GL_TEXTURE_2D, t.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t.width, t.height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, img->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	log_message("Loaded texture %s", name);
	if (hash_map_add(&s_textures, &t) == SIZE_MAX)
		return 0;

	return t.key;
}

void texture_unload(const texture_h h)
{
	const size_t index = hash_map_get_index(&s_textures, h);
	if (index == SIZE_MAX)
	{
		log_warning("Texture not found, so can't unload!");
		return;
	}

	const texture t = s_texture_entries[index];
	glDeleteProgram(t.id);
	hash_map_remove(&s_textures, index);
}

texture_h texture_get(const char* name)
{
	const hash h = hash_string(name);
	const size_t index = hash_map_get_index(&s_textures, h);

	if (index == SIZE_MAX)
	{
		log_message("Texture %s not found, so loading instead!", name);
		return texture_load(name);
	}
	return s_texture_entries[index].key;
}

void texture_set(const texture_h t, const int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture_get_id(t));
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