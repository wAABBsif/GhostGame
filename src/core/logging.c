#include "logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void s_log_vraw(const char *format, va_list args)
{
	vprintf(format, args);
}

void log_raw(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	s_log_vraw(format, args);
}

// void LOG_MESSAGE(const char *format, ...)
// {
// 	va_list list;
// 	va_start(list, format);
//
// 	_log_raw("[MESSAGE] ");
// 	_log_vraw(format, list);
// 	_log_raw("\n");
// }
//
// void LOG_WARNING(const char *format, ...)
// {
// 	va_list list;
// 	va_start(list, format);
//
// 	SET_CONSOLE_TEXT_COLOR_YELLOW();
// 	_log_raw("[WARNING] ");
// 	_log_vraw(format, list);
// 	_log_raw("\n");
// 	SET_CONSOLE_TEXT_COLOR_WHITE();
// }
//
// void LOG_ERROR(const char *format, ...)
// {
// 	va_list list;
// 	va_start(list, format);
//
// 	SET_CONSOLE_TEXT_COLOR_RED();
// 	_log_raw("[ERROR] ");
// 	_log_vraw(format, list);
// 	_log_raw("\n");
// 	SET_CONSOLE_TEXT_COLOR_WHITE();
//
// #if !defined(IS_DEBUG)
// 	abort();
// #endif
// }