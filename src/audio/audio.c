#include "audio.h"
#include "sfx.h"
#include "music.h"
#include "core/hash_map.h"
#include "core/logging.h"

#include "SDL3_mixer/SDL_mixer.h"

typedef struct audio_clip
{
	audio_clip_h hash;
	MIX_Audio *audio;
} audio_clip;

static hash_map s_clips;
static MIX_Mixer *s_sdl_mixer;
static MIX_Track *s_tracks[MAX_AUDIO_TRACKS];

void audio_init(void)
{
	log_message("Initializing audio...");

	MIX_Init();
	s_sdl_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

	hash_map_create(&s_clips, sizeof(audio_clip), MAX_AUDIO_CLIPS);

	sfx_init();
	music_init();

	const audio_clip_h clip = audio_clip_load("res/audio/test.wav", true);
	const audio_track_h track = audio_create_track();
	audio_set_track(track, clip);
	audio_play_track(track, false);
}

void audio_update(void)
{
	sfx_update();
	music_update();
}

void audio_terminate(void)
{
	log_message("Terminating audio...");

	for (int i = 0; i < s_clips.size; i++)
	{
		audio_clip *c = hash_map_index(&s_clips, i);
		MIX_DestroyAudio(c->audio);
		c->audio = NULL;
	}

	hash_map_destroy(&s_clips);

	sfx_terminate();
	music_terminate();

	MIX_Quit();
}

audio_clip_h audio_clip_load(const char* name, const bool decode)
{
	audio_clip clip;

	if (s_clips.size >= MAX_AUDIO_CLIPS)
	{
		log_warning("Attempted to add sound clip when there are already too many sound clips!");
		return 0;
	}

	clip.audio = MIX_LoadAudio(s_sdl_mixer, name, decode);
	if (clip.audio == NULL)
	{
		log_error("Failed to load audio: %s", name);
		return 0;
	}

	clip.hash = hash_string(name);
	log_message("Loaded audio clip: %s", name);
	if (hash_map_add(&s_clips, &clip) == SIZE_MAX)
		return 0;

	return clip.hash;
}

void audio_clip_unload(const audio_clip_h h)
{
	const size_t index = hash_map_get_index(&s_clips, h);
	if (index == SIZE_MAX)
	{
		log_warning("Audio clip not found, so can't unload!");
		return;
	}

	const audio_clip clip = *(audio_clip *)hash_map_index(&s_clips, h);
	MIX_DestroyAudio(clip.audio);
	hash_map_remove(&s_clips, index);
}

audio_clip_h audio_clip_get(const char* name)
{
	const hash h = hash_string(name);
	const size_t index = hash_map_get_index(&s_clips, h);

	if (index == SIZE_MAX)
	{
		log_warning("Audio clip %s not found!", name);
		return 0;
	}

	return ((audio_clip *)hash_map_index(&s_clips, index))->hash;
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
	if (!audio_track_is_valid(track))
		return log_warning("Invalid audio track cannot be destroyed");

	MIX_DestroyTrack(s_tracks[track]);
	s_tracks[track] = NULL;
}

bool audio_track_is_valid(const audio_track_h track)
{
	return s_tracks[track] != NULL && track < MAX_AUDIO_TRACKS && track >= 0;
}

void audio_set_track(const audio_track_h track, const audio_clip_h clip)
{
	const audio_clip *c = hash_map_index(&s_clips, hash_map_get_index(&s_clips, clip));
	if (c == NULL)
		return log_warning("Invalid audio clip cannot be used.");

	MIX_SetTrackAudio(s_tracks[track], c->audio);
}

void audio_play_track(const audio_track_h track, const bool loop)
{
	SDL_PropertiesID properties = 0;
	if (loop)
		properties = SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
	MIX_PlayTrack(s_tracks[track], properties);
}

void audio_pause_track(const audio_track_h track)
{
	MIX_PauseTrack(s_tracks[track]);
}

void audio_stop_track(const audio_track_h track)
{
	MIX_StopTrack(s_tracks[track], 0);
}

void audio_set_track_volume(const audio_track_h track, const float volume)
{
	MIX_SetTrackGain(s_tracks[track], volume);
}