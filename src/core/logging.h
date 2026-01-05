#pragma once

#include <stdio.h>
#include <string.h>

void log_init();
void log_raw(const char *format, ...);
void log_flush();

#if defined(WIN32)
#include <windows.h>
#define SET_CONSOLE_TEXT_COLOR_WHITE()													\
{																						\
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);										\
SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	\
}
#define SET_CONSOLE_TEXT_COLOR_YELLOW()													\
{																						\
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);										\
SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);					\
}
#define SET_CONSOLE_TEXT_COLOR_RED()													\
{																						\
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);										\
SetConsoleTextAttribute(hConsole, FOREGROUND_RED);										\
}
#elif defined(linux)
#define SET_CONSOLE_TEXT_COLOR_WHITE()	printf("\x1b[0m")
#define SET_CONSOLE_TEXT_COLOR_YELLOW()	printf("\x1b[33m")
#define SET_CONSOLE_TEXT_COLOR_RED()	printf("\x1b[31m")
#endif

#ifndef SET_CONSOLE_TEXT_COLOR_WHITE
#define SET_CONSOLE_TEXT_COLOR_WHITE()
#define SET_CONSOLE_TEXT_COLOR_YELLOW()
#define SET_CONSOLE_TEXT_COLOR_RED()
#endif

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
	SET_CONSOLE_TEXT_COLOR_YELLOW();													\
	log_raw(message_start);																\
	log_raw(format, ##__VA_ARGS__);														\
	log_raw("\n");																		\
	log_flush();																		\
	SET_CONSOLE_TEXT_COLOR_WHITE();														\
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
	SET_CONSOLE_TEXT_COLOR_RED();														\
	log_raw(message_start);																\
	log_raw(format, ##__VA_ARGS__);														\
	log_raw("\n");																		\
	log_flush();																		\
	SET_CONSOLE_TEXT_COLOR_WHITE();														\
})
