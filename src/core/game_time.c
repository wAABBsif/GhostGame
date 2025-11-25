#include <stdint.h>

#include "game_time.h"

#include <math.h>

#include "logging.h"
#include "SDL3/SDL_timer.h"

static float s_nanoseconds_to_seconds(const uint64_t nanoseconds)
{
	return nanoseconds / 1000000000.0f;
}

float s_prev_time;
float s_current_time;
float s_elapsed_time;

void game_time_init(void)
{
	s_prev_time = 0;
	s_current_time = 0;
}

void game_time_update(void)
{
	s_prev_time = s_current_time;
	s_current_time = s_nanoseconds_to_seconds(SDL_GetTicksNS());

	s_elapsed_time += game_time_get_delta();
}

float game_time_get_elapsed(void)
{
	return s_elapsed_time;
}

float game_time_get_delta(void)
{
	return fmin(s_current_time - s_prev_time, MAX_DELTA_TIME);
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