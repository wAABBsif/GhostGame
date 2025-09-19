#include <stdint.h>

#include "game_time.h"

#include "logging.h"
#include "SDL3/SDL_timer.h"

static float s_nanoseconds_to_seconds(const uint64_t nanoseconds)
{
	return nanoseconds / 1000000000.0f;
}

const float FPS_TIME_INTERVAL = 0.05f;
static float s_fps_timer;
static float s_fps_points[10];

float s_prev_elapsed_time;
float s_elapsed_time;

void game_time_init(void)
{
	s_prev_elapsed_time = 0;
	s_elapsed_time = 0;
	s_fps_timer = 0;
	for (int i = 0; i < sizeof(s_fps_points) / sizeof(s_fps_points[0]); i++)
	{
		s_fps_points[i] = 0;
	}
}

void game_time_update(void)
{
	s_prev_elapsed_time = s_elapsed_time;
	s_elapsed_time = s_nanoseconds_to_seconds(SDL_GetTicksNS());

	s_fps_timer -= game_time_get_delta();
	if (s_fps_timer <= 0)
	{
		s_fps_timer += FPS_TIME_INTERVAL;

		for (int i = sizeof(s_fps_points) / sizeof(s_fps_points[0]) - 1; i >= 1; i--)
			s_fps_points[i] = s_fps_points[i - 1];

		s_fps_points[0] = 1.0f / game_time_get_delta();
	}
}

float game_time_get_elapsed(void)
{
	return s_elapsed_time;
}

float game_time_get_delta(void)
{
	return s_elapsed_time - s_prev_elapsed_time;
}

float game_time_get_fps(void)
{
	float fps = 0.0f;
	const int size = sizeof(s_fps_points) / sizeof(s_fps_points[0]);

	for (int i = 0; i < size; i++)
	{
		fps += s_fps_points[i];
	}

	fps /= (float)size;
	return fps;
}

void game_timer_start(game_timer *timer)
{
	timer->start = s_nanoseconds_to_seconds(SDL_GetTicksNS());
}

void game_timer_end(game_timer *timer)
{
	timer->end = s_nanoseconds_to_seconds(SDL_GetTicksNS());
}

float game_timer_get_start(const game_timer *timer)
{
	return timer->start;
}

float game_timer_get_end(const game_timer *timer)
{
	return timer->end;
}

float game_timer_get_elapsed(const game_timer *timer)
{
	return timer->end - timer->start;
}