#include "audio.h"
#include "sfx.h"
#include "music.h"
#include "core/logging.h"

#include "SDL3_mixer/SDL_mixer.h"

static MIX_Mixer *s_sdl_mixer;
static MIX_Track *s_tracks[MAX_AUDIO_TRACKS];

void audio_init(void)
{
	log_message("Initializing audio...");

	MIX_Init();
	s_sdl_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

	sfx_init();
	music_init();
}

void audio_update(void)
{
	sfx_update();
	music_update();
}

void audio_terminate(void)
{
	log_message("Terminating audio...");

	sfx_terminate();
	music_terminate();

	MIX_Quit();
}

audio_track_h audio_create_track(void)
{
	for (int i = 0; i < MAX_AUDIO_TRACKS; i++)
	{
		if (s_tracks[i] != NULL)
			continue;

		s_tracks[i] = MIX_CreateTrack(s_sdl_mixer);
		return i;
	}

	log_error("Failed to create audio track");
	return -1;
}

void audio_destroy_track(const audio_track_h track)
{
	if (s_tracks[track] == NULL || track >= MAX_AUDIO_TRACKS)
		return log_warning("Invalid audio track cannot be destroyed");

	MIX_DestroyTrack(s_tracks[track]);
	s_tracks[track] = NULL;
}