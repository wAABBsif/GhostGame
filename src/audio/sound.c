#include "sound.h"


#include "AL/al.h"
#include "core/game_assert.h"
#include "core/hash_map.h"
#include "core/logging.h"
#include "SDL3/SDL_audio.h"

typedef struct sound
{
	sound_h key;
	sound_id id;
} sound;

static hash_map s_sounds;
static sound s_sound_entries[MAX_SOUNDS];

void sounds_init(void)
{
	LOG_MESSAGE("Initializing sounds...");
	hash_map_create(&s_sounds, sizeof(sound), MAX_SOUNDS, s_sound_entries);
}

void sounds_clear(void)
{
	LOG_MESSAGE("Clearing sounds...");
	for (int i = 0; i < s_sounds.size; i++)
	{
		alDeleteBuffers(1, &s_sound_entries[i].id);
	}

	hash_map_destroy(&s_sounds);
}

sound_h sound_load(const char* name)
{
	if (s_sounds.size >= MAX_SOUNDS)
	{
		LOG_WARNING("Attempted to add sound when there are already too many sounds!");
		return 0;
	}

	SDL_AudioSpec audio_spec;
	uint8_t *audio_buffer;
	uint32_t audio_buffer_size;
	const bool wav_result = SDL_LoadWAV(name, &audio_spec, &audio_buffer, &audio_buffer_size);
	if (!wav_result)
	{
		LOG_ERROR("Failed to load audio file: %s", name);
		return 0;
	}

	if (audio_spec.format != SDL_AUDIO_S16LE && audio_spec.format != SDL_AUDIO_S8 || audio_spec.channels > 2)
	{
		LOG_ERROR("Unsupported audio format: %s", name);
		return 0;
	}

	sound s;
	s.key = hash_string(name);

	ALenum format = audio_spec.channels == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
	format += audio_spec.format == SDL_AUDIO_S16LE ? 1 : 0;

	alGenBuffers(1, &s.id);
	alBufferData(s.id, format, audio_buffer, (int)audio_buffer_size, audio_spec.freq);
	SDL_free(audio_buffer);

	LOG_MESSAGE("Loaded sound %s", name);
	const size_t add_result = hash_map_add(&s_sounds, &s);
	GAME_ASSERT(add_result != HASH_INVALID);

	return s.key;
}

void sound_unload(sound_h h)
{
	const size_t index = hash_map_get_index(&s_sounds, h);
	GAME_ASSERT(index != HASH_INVALID);
	const sound t = s_sound_entries[index];
	alDeleteBuffers(1, &t.id);
	hash_map_remove(&s_sounds, index);
}

sound_h sound_get(const char* name)
{
	const hash h = hash_string(name);
	GAME_ASSERT(hash_map_get_index(&s_sounds, h) != HASH_INVALID);
	return h;
}

int32_t sound_get_buffer(const sound_h h)
{
	const size_t index = hash_map_get_index(&s_sounds, h);
	GAME_ASSERT(index != HASH_INVALID);
	return s_sound_entries[index].id;
}