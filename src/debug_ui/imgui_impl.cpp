#if defined(IS_DEBUG)
#include "imgui_impl.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl3.h"
#include "imgui/backends/imgui_impl_opengl3.h"

void imgui_create_context()
{
	ImGui::CreateContext();
}

imgui_io *imgui_get_io()
{
	return reinterpret_cast<imgui_io*>(&ImGui::GetIO());
}

void imgui_impl_sdl3_init_for_opengl(void *window, void *context)
{
	ImGui_ImplSDL3_InitForOpenGL(static_cast<SDL_Window*>(window), context);
}

void imgui_impl_opengl3_init()
{
	ImGui_ImplOpenGL3_Init();
}

void imgui_impl_sdl3_process_event(void *event)
{
	ImGui_ImplSDL3_ProcessEvent(static_cast<SDL_Event*>(event));
}

void imgui_impl_opengl3_new_frame()
{
	ImGui_ImplOpenGL3_NewFrame();
}

void imgui_impl_sdl3_new_frame()
{
	ImGui_ImplSDL3_NewFrame();
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

void imgui_impl_opengl3_render_draw_data()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui_impl_opengl3_shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
}

void imgui_impl_sdl3_shutdown()
{
	ImGui_ImplSDL3_Shutdown();
}

void imgui_destroy_context()
{
	ImGui::DestroyContext();
}

void imgui_dock_space_over_viewport()
{
	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}

void imgui_show_demo_window()
{
	ImGui::ShowDemoWindow();
}

void imgui_update_platform_windows()
{
	ImGui::UpdatePlatformWindows();
}

void imgui_render_platform_windows_default()
{
	ImGui::RenderPlatformWindowsDefault();
}

#endif