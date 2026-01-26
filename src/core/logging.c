#include "logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug_ui/log_window.h"

#if defined(WIN32)
#include <windows.h>
#endif

static FILE *s_log_file = NULL;

static void s_log_begin_file(const char *filename)
{
	s_log_file = fopen(filename, "w");
}

void log_init()
{
	s_log_begin_file("log.txt");
}

void log_flush()
{
	fflush(s_log_file);
	log_window_new_message();
}

void log_raw(const char *format, ...)
{
	va_list args;
	va_start(args, format);

	static char s_temp_buffer[LOG_MESSAGE_MAX_SIZE];
	const size_t size = vsprintf(s_temp_buffer, format, args);
	va_end(args);

	memset(s_temp_buffer + size, 0, LOG_MESSAGE_MAX_SIZE - size);

	printf("%s", s_temp_buffer);
	fprintf(s_log_file, "%s", s_temp_buffer);
	log_window_append_message(s_temp_buffer);
}

void set_console_text_color(log_color color)
{
	log_window_set_message_color(color);
#if defined(WIN32)
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (color == LOG_COLOR_WHITE)
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	else if (color == LOG_COLOR_YELLOW)
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
	else if (color == LOG_COLOR_RED)
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
#elif defined(linux)
	if (color == LOG_COLOR_WHITE)
		printf("\x1b[0m");
	else if (color == LOG_COLOR_YELLOW)
		printf("\x1b[33m");
	else if (color == LOG_COLOR_RED)
		printf("\x1b[31m");
#endif
}