#include "music.h"

#include <stdint.h>

#include "vorbis_implementation.h"

#include "AL/al.h"
#include "core/logging.h"

static stb_vorbis *s_stream;
static ALenum s_format;
static uint32_t s_sample_rate;

static uint32_t s_buffers[MUSIC_BUFFER_COUNT];
static uint32_t s_source;
static short s_current_buffer[MUSIC_BUFFER_SAMPLE_COUNT];

void music_init(void)
{
	LOG_MESSAGE("Initializing music...");

	alGenBuffers(MUSIC_BUFFER_COUNT, s_buffers);
	alGenSources(1, &s_source);
}

void music_update(void)
{

}

void music_clear(void)
{
	LOG_MESSAGE("Clearing music...");

	alDeleteBuffers(MUSIC_BUFFER_COUNT, s_buffers);
	alDeleteSources(1, &s_source);
	if (s_stream != NULL)
		stb_vorbis_close(s_stream);
}

bool music_play(const char* name)
{
	alSourcei(s_source, AL_BUFFER, s_buffers[0]);
	alSourcef(s_source, AL_PITCH, 1.0f);
	alSourcef(s_source, AL_GAIN, 1.0f);
	alSource3f(s_source, AL_POSITION, 0, 0, 0);
	alSource3f(s_source, AL_VELOCITY, 0, 0, 0);
	alSourcei(s_source, AL_LOOPING, AL_FALSE);

	s_stream = stb_vorbis_open_filename(name, NULL, NULL);
	const stb_vorbis_info info = stb_vorbis_get_info(s_stream);
	if (info.channels > 2)
	{
		LOG_ERROR("Unsupported audio format: %s", name);
		return false;
	}

	s_format = info.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	s_sample_rate = info.sample_rate;

	stb_vorbis_get_samples_short_interleaved(s_stream, info.channels, s_current_buffer, MUSIC_BUFFER_SAMPLE_COUNT);
	alBufferData(s_buffers[0], s_format, s_current_buffer, MUSIC_BUFFER_SAMPLE_COUNT * sizeof(short), info.sample_rate);

	LOG_MESSAGE("Playing music %s", name);
	alSourcePlay(s_source);

	return true;
}

void music_stop(void)
{

}

void music_pause(void)
{

}

void music_resume(void)
{

}