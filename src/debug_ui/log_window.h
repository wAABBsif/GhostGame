#pragma once

#define LOG_WINDOW_BUFFER_SIZE	65536

void log_window_update();
void log_window_append_message(const char* text);