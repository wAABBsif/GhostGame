#if defined(IS_DEBUG)
#include "imgui_impl.h"

#include "gfx/color.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl3.h"
#include "imgui/backends/imgui_impl_opengl3.h"

imgui_io *imgui_get_io()
{
	return reinterpret_cast<imgui_io*>(&ImGui::GetIO());
}

void *imgui_create_context(void *shared_font_atlas)
{
	return ImGui::CreateContext(static_cast<ImFontAtlas *>(shared_font_atlas));
}

void imgui_destroy_context(void *ctx)
{
	ImGui::DestroyContext(static_cast<ImGuiContext *>(ctx));
}

bool imgui_opengl3_init(const char *glsl_version)
{
	return ImGui_ImplOpenGL3_Init(glsl_version);
}

void imgui_opengl3_new_frame()
{
	ImGui_ImplOpenGL3_NewFrame();
}

void imgui_impl_opengl3_render_draw_data(void *draw_data)
{
	ImGui_ImplOpenGL3_RenderDrawData(static_cast<ImDrawData *>(draw_data));
}

void imgui_impl_opengl3_shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
}

bool imgui_sdl3_init_for_opengl(void *window, void *context)
{
	return ImGui_ImplSDL3_InitForOpenGL(static_cast<SDL_Window*>(window), context);
}

void imgui_sdl3_new_frame()
{
	ImGui_ImplSDL3_NewFrame();
}

bool imgui_sdl3_process_event(void *event)
{
	return ImGui_ImplSDL3_ProcessEvent(static_cast<SDL_Event*>(event));
}

void imgui_sdl3_shutdown()
{
	ImGui_ImplSDL3_Shutdown();
}

void *imgui_get_draw_data()
{
	return ImGui::GetDrawData();
}

void imgui_new_frame()
{
	ImGui::NewFrame();
}

void imgui_end_frame()
{
	ImGui::EndFrame();
}

void imgui_render()
{
	ImGui::Render();
}

imgui_id imgui_dock_space_over_viewport()
{
	return ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}

void imgui_update_platform_windows()
{
	ImGui::UpdatePlatformWindows();
}

void imgui_render_platform_windows_default(void *platform_render_arg, void *renderer_render_arg)
{
	ImGui::RenderPlatformWindowsDefault(platform_render_arg, renderer_render_arg);
}

void imgui_show_demo_window(bool *p_open)
{
	ImGui::ShowDemoWindow(p_open);
}

bool imgui_begin(const char *name, bool *p_open, const imgui_window_flags flags)
{
	return ImGui::Begin(name, p_open, flags);
}

void imgui_end()
{
	ImGui::End();
}

void imgui_text(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	ImGui::TextV(fmt, args);
	va_end(args);
}

void imgui_bullet_text(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	ImGui::BulletTextV(fmt, args);
	va_end(args);
}

bool imgui_begin_child(const char* str_id, const vec2 size, const imgui_child_flags child_flags, const imgui_window_flags window_flags)
{
	const auto im_size = ImVec2(size.x, size.y);
	return ImGui::BeginChild(str_id, im_size, child_flags, window_flags);
}

void imgui_end_child()
{
	ImGui::EndChild();
}

void imgui_push_style_color(const imgui_col col, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
	ImGui::PushStyleColor(col, IM_COL32(r, g, b, a));
}

void imgui_pop_style_color(const int count)
{
	ImGui::PopStyleColor(count);
}

#endif