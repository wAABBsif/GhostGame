#pragma once
#include "logging.h"

#define MAX_DELTA_TIME (1.0f / 60.0f)

#define QUICK_PERFORMANCE_TIMER_START(name) \
	game_timer name; \
	game_timer_start(&name) \

#define QUICK_PERFORMANCE_TIMER_END(name) \
game_timer_end(&name); \
log_message("Timer [%s]: %f", #name, game_timer_get_elapsed(&name))

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