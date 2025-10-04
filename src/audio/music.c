#include "music.h"
#include "core/logging.h"
#include "SDL3_mixer/SDL_mixer.h"

static float s_global_volume = 1;
static audio_track_h s_track;

void music_init(void)
{
	log_message("Initializing music...");

	s_track = audio_create_track();
	music_set_volume(1);
}

void music_update(void)
{

}

void music_terminate(void)
{
	log_message("Terminating music...");
	audio_destroy_track(s_track);
}

void music_set_global_volume(const float volume)
{
	s_global_volume = volume;
}

float music_get_global_volume(void)
{
	return s_global_volume;
}

void music_set(const audio_clip_h clip)
{
	audio_set_track_clip(s_track, clip);
}

void music_play(const bool loop)
{
	audio_play_track(s_track, loop);
}

void music_pause()
{
	audio_pause_track(s_track);
}

void music_stop()
{
	audio_stop_track(s_track);
}

void music_set_volume(const float volume)
{
	audio_set_track_volume(s_track, volume * s_global_volume);
}

float music_get_volume(void)
{
	return audio_get_track_volume(s_track) / s_global_volume;
}