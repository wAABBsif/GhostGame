#pragma once

#define LOG_WINDOW_MAX_MESSAGES	256
#include "core/logging.h"

void log_window_update();
void log_window_append_message(const char* text);
void log_window_new_message();
void log_window_set_message_color(log_color color);