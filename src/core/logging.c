#include "logging.h"

#include <stdarg.h>
#include <stdio.h>

#if defined(WIN32)
#include <windows.h>
#define SET_CONSOLE_TEXT_COLOR_WHITE() \
	{	\
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	\
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	\
	}
#define SET_CONSOLE_TEXT_COLOR_YELLOW()	\
	{	\
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	\
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);	\
	}
#define SET_CONSOLE_TEXT_COLOR_RED() \
	{	\
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	\
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);	\
	}
#endif

#ifndef SET_CONSOLE_TEXT_COLOR_WHITE
#define SET_CONSOLE_TEXT_COLOR_WHITE()
#define SET_CONSOLE_TEXT_COLOR_YELLOW()
#define SET_CONSOLE_TEXT_COLOR_RED()
#endif


void _log_vraw(const char *format, const va_list args)
{
	vprintf(format, args);
}

void _log_raw(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	_log_vraw(format, args);
}

void log_message(const char *format, ...)
{
	va_list list;
	va_start(list, format);

	_log_raw("[MESSAGE] ");
	_log_vraw(format, list);
	_log_raw("\n");
}

void log_warning(const char *format, ...)
{
	va_list list;
	va_start(list, format);

	SET_CONSOLE_TEXT_COLOR_YELLOW();
	_log_raw("[WARNING] ");
	_log_vraw(format, list);
	_log_raw("\n");
	SET_CONSOLE_TEXT_COLOR_WHITE();
}

void log_error(const char *format, ...)
{
	va_list list;
	va_start(list, format);

	SET_CONSOLE_TEXT_COLOR_RED();
	_log_raw("[ERROR] ");
	_log_vraw(format, list);
	_log_raw("\n");
	SET_CONSOLE_TEXT_COLOR_WHITE();

#if !defined(IS_DEBUG)
	exit(-1);
#endif
}