#include "music.h"

#include <stdint.h>

#include "vorbis_implementation.h"

#include "AL/al.h"
#include "core/logging.h"

static stb_vorbis *s_stream;
static ALenum s_format;
static uint32_t s_sample_rate;
static int32_t s_channels;

static uint32_t s_buffers[MUSIC_BUFFER_COUNT];
static uint32_t s_source;
static short s_current_buffer[MUSIC_BUFFER_SAMPLE_COUNT];

void music_init(void)
{
	LOG_MESSAGE("Initializing music...");

	alGenBuffers(MUSIC_BUFFER_COUNT, s_buffers);
	alGenSources(1, &s_source);
	alSourcef(s_source, AL_PITCH, 1.0f);
	alSourcef(s_source, AL_GAIN, 1.0f);
	alSource3f(s_source, AL_POSITION, 0, 0, 0);
	alSource3f(s_source, AL_VELOCITY, 0, 0, 0);
	alSourcei(s_source, AL_LOOPING, AL_FALSE);
}

void music_update(void)
{
	int processed;
	alGetSourcei(s_source, AL_BUFFERS_PROCESSED, &processed);
	for (int i = 0; i < processed; i++)
	{
		uint32_t which;
		alSourceUnqueueBuffers(s_source, 1, &which);

		int actual_buffer_size = stb_vorbis_get_samples_short_interleaved(s_stream, s_channels, s_current_buffer, MUSIC_BUFFER_SAMPLE_COUNT);
		if (actual_buffer_size <= 0)
		{
			stb_vorbis_seek_start(s_stream);
			actual_buffer_size = stb_vorbis_get_samples_short_interleaved(s_stream, s_channels, s_current_buffer, MUSIC_BUFFER_SAMPLE_COUNT);
		}

		alBufferData(which, s_format, s_current_buffer, actual_buffer_size * sizeof(short) * 2, s_sample_rate);
		alSourceQueueBuffers(s_source, 1, &which);
	}
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
	s_stream = stb_vorbis_open_filename(name, NULL, NULL);
	const stb_vorbis_info info = stb_vorbis_get_info(s_stream);
	if (info.channels > 2)
	{
		LOG_ERROR("Unsupported audio format: %s", name);
		return false;
	}

	s_format = info.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	s_sample_rate = info.sample_rate;
	s_channels = info.channels;

	for (int i = 0; i < MUSIC_BUFFER_COUNT; i++)
	{
		const int actual_buffer_size = stb_vorbis_get_samples_short_interleaved(s_stream, s_channels, s_current_buffer, MUSIC_BUFFER_SAMPLE_COUNT);
		alBufferData(s_buffers[i], s_format, s_current_buffer, actual_buffer_size * sizeof(short) * 2, s_sample_rate);
	}

	int processed;
	alGetSourcei(s_source, AL_BUFFERS_PROCESSED, &processed);
	alSourceUnqueueBuffers(s_source, processed, NULL);
	alSourceQueueBuffers(s_source, MUSIC_BUFFER_COUNT, s_buffers);

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