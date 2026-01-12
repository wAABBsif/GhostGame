#include "audio.h"
#include "AL/alc.h"
#include <stdlib.h>

#include "audio_source.h"
#include "sound.h"
#include "core/logging.h"

static ALCdevice *s_device;
static ALCcontext *s_context;

void audio_init()
{
	LOG_MESSAGE("Initializing audio...");

	s_device = alcOpenDevice(NULL);
	if (s_device == NULL)
	{
		LOG_ERROR("Failed to open audio device");
		exit(EXIT_FAILURE);
	}

	s_context = alcCreateContext(s_device, NULL);
	if (s_context == NULL)
	{
		LOG_ERROR("Failed to create audio context");
		exit(EXIT_FAILURE);
	}

	alcMakeContextCurrent(s_context);

	sounds_init();
	audio_sources_init();
}

void audio_update()
{

}

void audio_terminate()
{
	LOG_MESSAGE("Terminating audio...");

	audio_sources_terminate();
	sounds_clear();

	alcDestroyContext(s_context);
	alcCloseDevice(s_device);
}