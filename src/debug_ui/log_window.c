#include "log_window.h"

#include "imgui_impl.h"
#include "core/logging.h"

#if defined(IS_DEBUG)
static char s_char_buffer[LOG_WINDOW_BUFFER_SIZE];
static size_t s_char_buffer_size = 1;
#endif

void log_window_update()
{
#if defined(IS_DEBUG)
	imgui_begin("Log", NULL, IMGUI_WINDOW_FLAGS_NONE);
	imgui_begin_child("log_box", VEC2_ZERO, IMGUI_CHILD_FLAGS_BORDERS, IMGUI_WINDOW_FLAGS_ALWAYS_VERTICAL_SCROLLBAR);
	imgui_text(s_char_buffer);
	imgui_end_child();
	imgui_end();
#endif
}

void log_window_append_message(const char* text)
{
#if defined(IS_DEBUG)
	const size_t size = strlen(text);
	s_char_buffer_size += size;
	if (s_char_buffer_size > LOG_WINDOW_BUFFER_SIZE)
	{
		memset(s_char_buffer, 0, LOG_WINDOW_BUFFER_SIZE);
		s_char_buffer_size -= LOG_WINDOW_BUFFER_SIZE + 1;
		LOG_WARNING("Log window buffer overflow!");
	}
	strcat(s_char_buffer, text);
#endif
}