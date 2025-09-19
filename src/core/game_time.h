#pragma once
typedef struct game_timer
{
	float start;
	float end;
} game_timer;

void game_time_init(void);
void game_time_update(void);

float game_time_get_elapsed(void);
float game_time_get_delta(void);
float game_time_get_fps(void);

void game_timer_start(game_timer *timer);
void game_timer_end(game_timer *timer);
float game_timer_get_start(const game_timer *timer);
float game_timer_get_end(const game_timer *timer);
float game_timer_get_elapsed(const game_timer *timer);