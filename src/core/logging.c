#include "logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

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
}

void log_raw(const char *format, ...)
{
	va_list args, args_file;
	va_start(args, format);
	va_copy(args_file, args);

	vprintf(format, args);
	va_end(args);
	vfprintf(s_log_file, format, args_file);
	va_end(args_file);
}