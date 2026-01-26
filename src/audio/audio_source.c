#include "audio_source.h"

#include <string.h>

#include "AL/al.h"
#include "core/logging.h"

static uint32_t s_sources[MAX_AUDIO_SOURCES];

void audio_sources_init(void)
{
	LOG_MESSAGE("Initializing audio sources...");

	alGenSources(MAX_AUDIO_SOURCES, s_sources);
}

void audio_sources_terminate(void)
{
	LOG_MESSAGE("Terminating audio sources...");

	alDeleteSources(MAX_AUDIO_SOURCES, s_sources);
}

source_id audio_source_create(void)
{
	for (source_id i = 0; i < MAX_AUDIO_SOURCES; i++)
	{
		int32_t state;
		alGetSourcei(s_sources[i], AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING)
			continue;

		alSourcef(s_sources[i], AL_PITCH, 1.0f);
		alSourcef(s_sources[i], AL_GAIN, 1.0f);
		alSource3f(s_sources[i], AL_POSITION, 0, 0, 0);
		alSource3f(s_sources[i], AL_VELOCITY, 0, 0, 0);
		alSourcei(s_sources[i], AL_LOOPING, AL_FALSE);
		return i;
	}

	return SOURCE_ID_INVALID;
}

void audio_source_play(const source_id source)
{
	alSourcePlay(s_sources[source]);
}

void audio_source_stop(const source_id source)
{
	alSourceStop(s_sources[source]);
}

void audio_source_set_sound(const source_id source, const sound_h sound)
{
	alSourcei(s_sources[source], AL_BUFFER, sound_get_buffer(sound));
}

void audio_source_set_volume(const source_id source, const float volume)
{
	alSourcef(s_sources[source], AL_GAIN, volume);
}

void audio_source_set_pitch(const source_id source, const float pitch)
{
	alSourcef(s_sources[source], AL_PITCH, pitch);
}

void audio_source_set_position(const source_id source, const vec2 position)
{
	alSource3f(s_sources[source], AL_POSITION, position.x, 0, position.y);
}

void audio_source_set_velocity(const source_id source, const vec2 velocity)
{
	alSource3f(s_sources[source], AL_VELOCITY, velocity.x, 0, velocity.y);
}