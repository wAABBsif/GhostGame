#include "debug_ui.h"
#include "imgui_impl.h"
#include "inspector_window.h"
#include "log_window.h"
#include "objects_window.h"
#include "core/game_assert.h"
#include "core/game_time.h"
#include "core/logging.h"
#include "gfx/gfx.h"
#include "gfx/window.h"
#include "SDL3/SDL_events.h"

#if defined(IS_DEBUG)
static imgui_io *s_imgui_io;
static bool s_is_enabled = false;
#endif

void debug_ui_init()
{
#if defined(IS_DEBUG)
	LOG_MESSAGE("Initializing Debug UI...");

	imgui_create_context(NULL);
	s_imgui_io = imgui_get_io();
	s_imgui_io->ConfigFlags |= IMGUI_CONFIG_FLAGS_NAV_ENABLE_KEYBOARD;
	s_imgui_io->ConfigFlags |= IMGUI_CONFIG_FLAGS_DOCKING_ENABLE;

	window_initialize_for_debug_ui(gfx_get_window());
	imgui_opengl3_init(NULL);
#endif
}

void debug_ui_terminate()
{
#if defined(IS_DEBUG)
	LOG_MESSAGE("Terminating Debug UI...");

	imgui_impl_opengl3_shutdown();
	imgui_sdl3_shutdown();
	imgui_destroy_context(NULL);
#endif
}

void debug_ui_update()
{
#if defined(IS_DEBUG)
	imgui_opengl3_new_frame();
	imgui_sdl3_new_frame();
	imgui_new_frame();
	imgui_dock_space_over_viewport();

	if (!s_is_enabled)
		return;

	log_window_update();
	objects_window_update();
	inspector_window_update();
#endif
}

void debug_ui_draw()
{
#if defined(IS_DEBUG)
	imgui_render();
	imgui_impl_opengl3_render_draw_data(imgui_get_draw_data());
#endif
}

void debug_ui_handle_sdl_event(const SDL_Event *event)
{
#if defined(IS_DEBUG)
	GAME_ASSERT(event);

	imgui_sdl3_process_event((void *)event);

	if (event->type == SDL_EVENT_KEY_DOWN)
		if (event->key.key == SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1))
			s_is_enabled = !s_is_enabled;
#endif
}