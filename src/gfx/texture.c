#include "texture.h"

#include "core/hash_map.h"
#include "core/logging.h"
#include "glad/glad.h"
#include "SDL3_image/SDL_image.h"

const int MAX_TEXTURES = 64;

static hash s_active_texture;
static hash_map s_textures;

typedef struct texture
{
	hash key;
	texture_id id;
	uint32_t width;
	uint32_t height;
} texture;

void texture_init(void)
{
	log_message("Initializing textures...");
	hash_map_create(&s_textures, sizeof(texture), MAX_TEXTURES);
	s_active_texture = 0;
}

void texture_clear(void)
{
	log_message("Clearing textures...");
	for (int i = 0; i < s_textures.size; i++)
	{
		const texture *s = (texture *)hash_map_index(&s_textures, i);
		glDeleteTextures(1, &s->id);
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
	if (hash_map_add(&s_textures, &t) < 0)
		return 0;

	return t.key;
}

void texture_unload(const hash h)
{
	const size_t index = hash_map_get_index(&s_textures, h);
	if (index == -1)
	{
		log_warning("Texture not found, so can't unload!");
		return;
	}

	const texture t = *(texture*)hash_map_index(&s_textures, h);
	glDeleteProgram(t.id);
	hash_map_remove(&s_textures, index);
}

texture_h texture_get(const char* name)
{
	const hash h = hash_string(name);
	const ssize_t index = hash_map_get_index(&s_textures, h);

	if (index < 0)
	{
		log_warning("Texture %s not found, so loading instead!", name);
		return texture_load(name);
	}
	return ((texture *)hash_map_index(&s_textures, index))->key;
}

void texture_set(const texture_h t, const int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture_get_id(t));
}

texture_id texture_get_id(const texture_h t)
{
	const texture *tex = (texture *)hash_map_index(&s_textures, hash_map_get_index(&s_textures, t));
	return tex->id;
}