#include "audio.h"

#include <assert.h>

#include "core/game_time.h"
#include "core/hash_map.h"
#include "core/logging.h"
#include "core/mat3.h"
#include "core/vec2.h"
#include "gfx/camera.h"

#include "SDL3_mixer/SDL_mixer.h"

typedef struct audio_clip
{
	audio_clip_h hash;
	MIX_Audio *audio;
} audio_clip;

static hash_map s_clips;
static audio_clip s_clip_entries[MAX_AUDIO_CLIPS];
static MIX_Mixer *s_sdl_mixer;
static MIX_Track *s_tracks[MAX_AUDIO_TRACKS];

audio_track_h t;

void audio_init(void)
{
	log_message("Initializing audio...");

	MIX_Init();
	s_sdl_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

	hash_map_create(&s_clips, sizeof(audio_clip), MAX_AUDIO_CLIPS, s_clip_entries);
}

void audio_update(void)
{

}

void audio_terminate(void)
{
	log_message("Terminating audio...");

	for (int i = 0; i < s_clips.size; i++)
	{
		audio_clip *c = &s_clip_entries[i];
		MIX_DestroyAudio(c->audio);
		c->audio = NULL;
	}

	hash_map_destroy(&s_clips);

	MIX_Quit();
}

audio_clip_h audio_clip_load(const char* name, const bool is_sfx)
{
	audio_clip clip;
	assert(s_clips.size < MAX_AUDIO_CLIPS);

	clip.audio = MIX_LoadAudio(s_sdl_mixer, name, is_sfx);
	if (clip.audio == NULL)
	{
		log_error("Failed to load audio: %s", name);
		return 0;
	}

	clip.hash = hash_string(name);
	log_message("Loaded audio clip: %s", name);
	const size_t add_result = hash_map_add(&s_clips, &clip);
	assert(add_result != SIZE_MAX);

	return clip.hash;
}

void audio_clip_unload(const audio_clip_h h)
{
	const size_t index = hash_map_get_index(&s_clips, h);
	assert(index != SIZE_MAX);

	const audio_clip clip = s_clip_entries[index];
	MIX_DestroyAudio(clip.audio);
	hash_map_remove(&s_clips, index);
}

audio_clip_h audio_clip_get(const char* name)
{
	const hash h = hash_string(name);
	const size_t index = hash_map_get_index(&s_clips, h);
	assert(index != SIZE_MAX);

	return s_clip_entries[index].hash;
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

	assert(false);
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

bool audio_track_is_playing(const audio_track_h track)
{
	return MIX_TrackPlaying(s_tracks[track]);
}

void audio_set_track_clip(const audio_track_h track, const audio_clip_h clip)
{
	const audio_clip c = s_clip_entries[hash_map_get_index(&s_clips, clip)];
	MIX_SetTrackAudio(s_tracks[track], c.audio);
}

void audio_play_track(const audio_track_h track, const bool loop)
{
	const SDL_PropertiesID properties = SDL_CreateProperties();
	if (loop)
		SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOPS_NUMBER, -1);

	MIX_PlayTrack(s_tracks[track], properties);

	SDL_DestroyProperties(properties);
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

float audio_get_track_volume(const audio_track_h track)
{
	return MIX_GetTrackGain(s_tracks[track]);
}

void audio_set_track_position(const audio_track_h track, vec2 position)
{
	position = vec2_transform(position, world_to_camera_matrix(get_main_camera()));
	MIX_SetTrack3DPosition(s_tracks[track], &(MIX_Point3D){position.x * AUDIO_3D_SCALE, position.y * AUDIO_3D_SCALE, 0});
}

vec2 audio_get_track_position(const audio_track_h track)
{
	MIX_Point3D p;
	MIX_GetTrack3DPosition(s_tracks[track], &p);
	const vec2 result = (vec2){p.x / AUDIO_3D_SCALE, p.y / AUDIO_3D_SCALE};
	return vec2_transform(result, camera_to_world_matrix(get_main_camera()));
}