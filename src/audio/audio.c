#include "audio.h"
#include "AL/alc.h"
#include <stdlib.h>

#include "audio_source.h"
#include "music.h"
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
	music_init();
	audio_sources_init();

	sound_h a = sound_load("res/audio/sound.wav");
	source_id s = audio_source_create();
	audio_source_set_sound(s, a);
	audio_source_play(s);

	music_play("res/audio/a.ogg");
}

void audio_update()
{

}

void audio_terminate()
{
	LOG_MESSAGE("Terminating audio...");

	audio_sources_terminate();
	music_clear();
	sounds_clear();

	alcDestroyContext(s_context);
	alcCloseDevice(s_device);
}