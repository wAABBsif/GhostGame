#include "log_window.h"

#include "imgui_impl.h"
#include "core/logging.h"

#if defined(IS_DEBUG)
typedef struct log_message
{
	log_color	color;
	char		text[LOG_MESSAGE_MAX_SIZE];
} log_message;

static log_message s_char_buffer[LOG_WINDOW_MAX_MESSAGES];
static int16_t s_char_buffer_size = 0;
static int16_t s_char_buffer_index = 0;
#endif

void log_window_update()
{
#if defined(IS_DEBUG)
	imgui_begin("Log", NULL, IMGUI_WINDOW_FLAGS_NONE);
	imgui_begin_child("log_box", VEC2_ZERO, IMGUI_CHILD_FLAGS_BORDERS, IMGUI_WINDOW_FLAGS_ALWAYS_VERTICAL_SCROLLBAR);
	for (int16_t i = 0; i < s_char_buffer_size; i++)
	{
		int16_t index = s_char_buffer_index - i;
		if (index < 0)
		{
			index = s_char_buffer_size + index;
		}
		if (s_char_buffer[index].color == LOG_COLOR_WHITE)
			imgui_push_style_color(IMGUI_COL_TEXT, 0xFF, 0xFF, 0xFF, 0xFF);
		else if (s_char_buffer[index].color == LOG_COLOR_YELLOW)
			imgui_push_style_color(IMGUI_COL_TEXT, 0xC0, 0xC0, 0x00, 0xFF);
		else if (s_char_buffer[index].color == LOG_COLOR_RED)
			imgui_push_style_color(IMGUI_COL_TEXT, 0xC0, 0x20, 0x20, 0xFF);
		imgui_text("%s", s_char_buffer[index].text);
		imgui_pop_style_color(1);
	}
	imgui_end_child();
	imgui_end();
#endif
}

void log_window_append_message(const char* text)
{
#if defined(IS_DEBUG)
	strcat(s_char_buffer[s_char_buffer_index].text, text);
#endif
}

void log_window_new_message()
{
#if defined(IS_DEBUG)
	if (s_char_buffer_size < LOG_WINDOW_MAX_MESSAGES)
		s_char_buffer_size++;

	s_char_buffer_index++;
	if (s_char_buffer_index >= LOG_WINDOW_MAX_MESSAGES)
		s_char_buffer_index = 0;
	s_char_buffer[s_char_buffer_index].text[0] = 0;
#endif
}

void log_window_set_message_color(const log_color color)
{
#if defined(IS_DEBUG)
	s_char_buffer[s_char_buffer_index].color = color;
#endif
}