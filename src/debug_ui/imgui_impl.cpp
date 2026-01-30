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

vec2 imgui_get_content_region_avail()
{
	const ImVec2 v = ImGui::GetContentRegionAvail();
	return (vec2){v.x, v.y};
}

void imgui_push_style_color(const imgui_col col, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
	ImGui::PushStyleColor(col, IM_COL32(r, g, b, a));
}

void imgui_pop_style_color(const int count)
{
	ImGui::PopStyleColor(count);
}

bool imgui_input_int(const char *label, int *v, const int step, const int step_fast, const imgui_input_text_flags flags)
{
	return ImGui::InputInt(label, v, step, step_fast, flags);
}

bool imgui_slider_int(const char *label, int *v, const int v_min, const int v_max, const char *format, const imgui_slider_flags flags)
{
	return ImGui::SliderInt(label, v, v_min, v_max, format, flags);
}

bool imgui_drag_int(const char *label, int *v, const float v_speed, const int v_min, const int v_max, const char *format, const imgui_slider_flags flags)
{
	return ImGui::DragInt(label, v, v_speed, v_min, v_max, format, flags);
}

bool imgui_drag_vec2u8(const char *label, vec2u8 *v, const float v_speed, const int v_min, const int v_max, const char *format, const imgui_slider_flags flags)
{
	int temp[2] = {v->x, v->y};
	const bool result = ImGui::DragInt2(label, temp, v_speed, v_min, v_max, format, flags);
	v->x = temp[0];
	v->y = temp[1];
	return result;
}

bool imgui_drag_vec2u16(const char *label, vec2u16 *v, const float v_speed, const int v_min, const int v_max, const char *format, const imgui_slider_flags flags)
{
	int temp[2] = {v->x, v->y};
	const bool result = ImGui::DragInt2(label, temp, v_speed, v_min, v_max, format, flags);
	v->x = temp[0];
	v->y = temp[1];
	return result;
}

bool imgui_drag_float(const char *label, float *v, const float v_speed, const float v_min, const float v_max, const char *format, const imgui_slider_flags flags)
{
	return ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, flags);
}

bool imgui_drag_vec2(const char *label, vec2 *v, const float v_speed, const float v_min, const float v_max, const char *format, const imgui_slider_flags flags)
{
	return ImGui::DragFloat2(label, reinterpret_cast<float*>(v), v_speed, v_min, v_max, format, flags);
}

bool imgui_slider_vec2(const char *label, vec2 *v, const float v_min, const float v_max, const char *format, const imgui_slider_flags flags)
{
	return ImGui::SliderFloat2(label, reinterpret_cast<float*>(v), v_min, v_max, format, flags);
}

bool imgui_color_edit(const char *label, color *c, const imgui_color_edit_flags flags)
{
	float v[4];
	v[0] = c->r / 255.0f;
	v[1] = c->g / 255.0f;
	v[2] = c->b / 255.0f;
	v[3] = c->a / 255.0f;

	const bool result = ImGui::ColorEdit4(label, v, flags);
	c->r = v[0] * 255;
	c->g = v[1] * 255;
	c->b = v[2] * 255;
	c->a = v[3] * 255;


	return result;
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

bool imgui_checkbox(const char *label, bool *v)
{
	return ImGui::Checkbox(label, v);
}

bool imgui_begin_combo(const char *label, const char *preview_value, imgui_combo_flags flags)
{
	return ImGui::BeginCombo(label, preview_value, flags);
}

void imgui_end_combo()
{
	ImGui::EndCombo();
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

bool imgui_collapsing_header(const char *label)
{
	return ImGui::CollapsingHeader(label);
}

bool imgui_begin_listbox(const char *label, vec2 size)
{
	const auto im_size = ImVec2(size.x, size.y);
	return ImGui::BeginListBox(label, im_size);
}

void imgui_end_listbox()
{
	ImGui::EndListBox();
}

bool imgui_selectable(const char *label, const bool selected, const imgui_selectable_flags flags, const vec2 size)
{
	const auto im_size = ImVec2(size.x, size.y);
	return ImGui::Selectable(label, selected, flags, im_size);
}

#endif