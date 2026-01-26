#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define LOG_MESSAGE_MAX_SIZE	255

#define LOG_COLOR_WHITE			0
#define LOG_COLOR_YELLOW		1
#define LOG_COLOR_RED			2

typedef uint8_t log_color;

void log_init();
void log_raw(const char *format, ...);
void log_flush();

void set_console_text_color(log_color color);

#define LOG_SPACE	48

#define LOG_MESSAGE(format, ...)														\
({																						\
	char message_start[LOG_SPACE];														\
	snprintf(message_start, LOG_SPACE, "[MESSAGE (%s:%d)]", __FILE_NAME__, __LINE__);	\
	const int length = strlen(message_start);											\
	for (int i = length; i < LOG_SPACE - 1; i++)										\
		message_start[i] = ' ';															\
	message_start[LOG_SPACE - 1] = 0;													\
																						\
	log_raw(message_start);																\
	log_raw(format, ##__VA_ARGS__);														\
	log_raw("\n");																		\
	log_flush();																		\
})

#define LOG_WARNING(format, ...)														\
({																						\
	char message_start[LOG_SPACE];														\
	snprintf(message_start, LOG_SPACE, "[WARNING (%s:%d)]", __FILE_NAME__, __LINE__);	\
	const int length = strlen(message_start);											\
	for (int i = length; i < LOG_SPACE - 1; i++)										\
		message_start[i] = ' ';															\
	message_start[LOG_SPACE - 1] = 0;													\
																						\
	set_console_text_color(LOG_COLOR_YELLOW);											\
	log_raw(message_start);																\
	log_raw(format, ##__VA_ARGS__);														\
	log_raw("\n");																		\
	log_flush();																		\
	set_console_text_color(LOG_COLOR_WHITE);											\
})

#define LOG_ERROR(format, ...)															\
({																						\
	char message_start[LOG_SPACE];														\
	snprintf(message_start, LOG_SPACE, "[ERROR (%s:%d)]", __FILE_NAME__, __LINE__);	\
	const int length = strlen(message_start);											\
	for (int i = length; i < LOG_SPACE - 1; i++)										\
		message_start[i] = ' ';															\
	message_start[LOG_SPACE - 1] = 0;													\
																						\
	set_console_text_color(LOG_COLOR_RED);												\
	log_raw(message_start);																\
	log_raw(format, ##__VA_ARGS__);														\
	log_raw("\n");																		\
	log_flush();																		\
	set_console_text_color(LOG_COLOR_WHITE);											\
})
