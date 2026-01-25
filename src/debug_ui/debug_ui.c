#include "debug_ui.h"
#include "imgui_impl.h"
#include "log_window.h"
#include "core/logging.h"
#include "gfx/gfx.h"
#include "gfx/window.h"

#if defined(IS_DEBUG)
static imgui_io *s_imgui_io;
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
	log_window_update();
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
	imgui_sdl3_process_event((void *)event);
#endif
}