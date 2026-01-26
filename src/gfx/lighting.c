#include "lighting.h"

#include <stdlib.h>
#include <string.h>

#include "camera.h"
#include "gfx.h"
#include "shader.h"
#include "window.h"
#include "core/game_assert.h"
#include "core/logging.h"
#include "core/mat3.h"
#include "core/vec2.h"
#include "glad/glad.h"

static uint32_t s_vao;
static uint32_t s_vbo;
static uint32_t s_ibo;
static shader_h s_shader;

static texture_h s_texture;

static light_quad s_quads[MAX_LIGHTS];
static uint16_t s_light_count;

void lighting_init(void)
{
	LOG_MESSAGE("Initializing lighting...");

	glGenVertexArrays(1, &s_vao);
	glBindVertexArray(s_vao);

	glGenBuffers(1, &s_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(light_quad) * MAX_LIGHTS, s_quads, GL_DYNAMIC_DRAW);

	//x, y
	glVertexAttribPointer(0, 2, GL_SHORT, false, sizeof(light_vertex), (void *)offsetof(light_vertex, x));
	glEnableVertexAttribArray(0);

	//texture_x, texture_y
	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, false, sizeof(light_vertex), (void *)offsetof(light_vertex, texture_x));
	glEnableVertexAttribArray(1);

	//color
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, true, sizeof(light_vertex), (void *)offsetof(light_vertex, color));
	glEnableVertexAttribArray(2);

	//type
	glVertexAttribIPointer(3, 1, GL_BYTE, sizeof(light_vertex), (void *)offsetof(light_vertex, type));
	glEnableVertexAttribArray(3);

	//priority
	glVertexAttribPointer(4, 1, GL_BYTE, true, sizeof(light_vertex), (void *)offsetof(light_vertex, priority));
	glEnableVertexAttribArray(4);

	//z
	glVertexAttribPointer(5, 1, GL_BYTE, true, sizeof(light_vertex), (void *)offsetof(light_vertex, z));
	glEnableVertexAttribArray(5);

	//intensity
	glVertexAttribPointer(6, 1, GL_UNSIGNED_BYTE, false, sizeof(light_vertex), (void *)offsetof(light_vertex, intensity_multiplier));
	glEnableVertexAttribArray(6);

	const uint16_t element_buffer_length = gfx_get_length_of_element_array(MAX_LIGHTS);
	uint16_t *element_indices = malloc(element_buffer_length * sizeof(uint16_t));
	gfx_generate_quad_elements(element_indices, MAX_LIGHTS);
	glGenBuffers(1, &s_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * element_buffer_length, element_indices, GL_STATIC_DRAW);
	free(element_indices);

	glBindVertexArray(0);

	s_texture = texture_load("res/radial_light.png");
	s_shader = shader_load("res/shaders/light");
}

void lighting_terminate(void)
{
	LOG_MESSAGE("Terminating lighting...");

	glDeleteVertexArrays(1, &s_vao);
	glDeleteBuffers(1, &s_vbo);
	glDeleteBuffers(1, &s_ibo);

	s_light_count = 0;
}

static uint16_t s_get_ordered_index(const int8_t z, const uint16_t start, const uint16_t end)
{
	if (start >= end)
		return start;

	const uint16_t mid = (start + end) / 2;
	const int8_t mid_z = s_quads[mid].vertices->priority;

	if (mid_z < z)
		return s_get_ordered_index(z, mid + 1, end);
	if (mid_z > z)
		return s_get_ordered_index(z, start, mid);
	return mid;
}

void add_light_quad(const light_quad *quad)
{
	GAME_ASSERT(s_light_count < MAX_LIGHTS);

	const uint16_t index = s_get_ordered_index(quad->vertices->priority, 0, s_light_count);
	memmove(s_quads + index + 1, s_quads + index, sizeof(light_quad) * (s_light_count - index));
	s_quads[index] = *quad;
	s_light_count++;
}

void draw_lighting(void)
{
	shader_set(s_shader);

	glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(light_quad) * s_light_count, s_quads);

	texture_set(texture_get_id(s_texture), 0);
	shader_set_int32_t(s_shader, "radial_texture", 0);

	texture_set(get_main_camera()->main_texture.depth_texture, 1);
	shader_set_int32_t(s_shader, "depth_texture", 1);

	const vec2 size = camera_get_render_size(get_main_camera());
	shader_set_vec2i(s_shader, "render_size", size.x, size.y);

	shader_set_mat3(s_shader, "world_to_screen_matrix", world_to_screen_matrix(get_main_camera()));

	glBindVertexArray(s_vao);
	glDrawElements(GL_TRIANGLES, s_light_count * 6, GL_UNSIGNED_SHORT, 0);

	s_light_count = 0;
}