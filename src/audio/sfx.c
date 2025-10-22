#include "sfx.h"
#include "core/logging.h"
#include "core/vec2.h"

static float s_global_volume = 1;
static audio_track_h s_tracks[MAX_SFX_TRACKS];

void sfx_init(void)
{
	log_message("Initializing SFX...");
	for (int i = 0; i < MAX_SFX_TRACKS; i++)
	{
		s_tracks[i] = audio_create_track();
		sfx_set_volume(s_tracks[i], 1);
	}
}

void sfx_update(void)
{

}

void sfx_terminate(void)
{
	log_message("Terminating SFX...");
	for (int i = 0; i < MAX_SFX_TRACKS; i++)
		audio_destroy_track(s_tracks[i]);
}

void sfx_set_global_volume(const float volume)
{
	s_global_volume = volume;
}

float sfx_get_global_volume(void)
{
	return s_global_volume;
}

audio_track_h sfx_set(const audio_clip_h clip)
{
	audio_track_h track = -1;

	for (int i = 0; i < MAX_SFX_TRACKS; i++)
	{
		if (!audio_track_is_playing(s_tracks[i]))
		{
			track = s_tracks[i];
			break;
		}
	}

	if (track >= 0)
		audio_set_track_clip(track, clip);
	else
		log_warning("Failed to set SFX! Too many SFX playing!");

	return track;
}

void sfx_play(const audio_track_h track)
{
	audio_play_track(track, false);
}

void sfx_pause(const audio_track_h track)
{
	audio_pause_track(track);
}

void sfx_stop(const audio_track_h track)
{
	audio_stop_track(track);
}

bool sfx_is_playing(const audio_track_h track)
{
	return audio_track_is_playing(track);
}

void sfx_set_volume(const audio_track_h track, const float volume)
{
	audio_set_track_volume(track, volume * s_global_volume);
}

float sfx_get_volume(const audio_track_h track)
{
	if (s_global_volume == 0)
		return 0;
	return audio_get_track_volume(track) / s_global_volume;
}

void sfx_set_position(const audio_track_h track, const vec2 position)
{
	return audio_set_track_position(track, position);
}

vec2 sfx_get_position(const audio_track_h track)
{
	return audio_get_track_position(track);
}