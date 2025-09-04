#include "logging.h"

#include <stdarg.h>
#include <stdio.h>

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
#ifndef IS_DEBUG
	return;
#endif

	va_list list;
	va_start(list, format);

	_log_raw("[MESSAGE] ");
	_log_vraw(format, list);
	_log_raw("\n");
}

void log_warning(const char *format, ...)
{
#ifndef IS_DEBUG
	return;
#endif

	va_list list;
	va_start(list, format);

	_log_raw("[WARNING] ");
	_log_vraw(format, list);
	_log_raw("\n");
}

void log_error(const char *format, ...)
{
#ifndef IS_DEBUG
	return;
#endif

	va_list list;
	va_start(list, format);

	_log_raw("[ERROR] ");
	_log_vraw(format, list);
	_log_raw("\n");
}