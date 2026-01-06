#include <math.h>
#include "camera.h"

#include <assert.h>
#include <stdlib.h>

#include "gfx/window.h"
#include "core/vec2.h"
#include "core/mat3.h"
#include "gfx/gfx.h"
#include "gfx/shader.h"
#include "glad/glad.h"
#include "SDL3/SDL_stdinc.h"

#define MIN_ASPECT_RATIO	(5.0f / 4.0f)
#define MAX_ASPECT_RATIO	(7.0f / 3.0f)

static uint32_t s_vertex_array;
static uint32_t s_vertex_buffer;
static uint32_t s_index_buffer;
static shader_h s_shader;
static camera *s_main_camera;

typedef struct camera_quad_vertex
{
	vec2 position;
	vec2 tex_coord;
} camera_quad_vertex;

static camera_quad_vertex s_vertices[] =
{
	{-1, -1, +0, +0},
	{+1, -1, +1, +0},
	{+1, +1, +1, +1},
	{-1, +1, +0, +1},
};

void camera_init()
{
	glGenVertexArrays(1, &s_vertex_array);
	glBindVertexArray(s_vertex_array);

	glGenBuffers(1, &s_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(camera_quad_vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(camera_quad_vertex), (void*)offsetof(camera_quad_vertex, tex_coord));
	glEnableVertexAttribArray(1);

	const uint16_t element_buffer_length = gfx_get_length_of_element_array(1);
	uint16_t *element_indices = malloc(element_buffer_length * sizeof(uint16_t));
	gfx_generate_quad_elements(element_indices, 1);
	glGenBuffers(1, &s_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * element_buffer_length, element_indices, GL_STATIC_DRAW);
	free(element_indices);

	glBindVertexArray(0);

	s_shader = shader_load("res/shaders/camera_render");
}

void camera_terminate()
{
	glDeleteVertexArrays(1, &s_vertex_array);
	glDeleteBuffers(1, &s_vertex_buffer);
	glDeleteBuffers(1, &s_index_buffer);
}

static vec2 get_matrix_scale(const float scale)
{
	float aspect = window_get_aspect_ratio(gfx_get_window());
	aspect = SDL_clamp(aspect, MIN_ASPECT_RATIO, MAX_ASPECT_RATIO);
	return (vec2){2 / scale, 2 / scale * aspect};
}

static vec2 get_matrix_scale_inverted(const float scale)
{
	float aspect = window_get_aspect_ratio(gfx_get_window());
	aspect = SDL_clamp(aspect, MIN_ASPECT_RATIO, MAX_ASPECT_RATIO);
	return (vec2){scale / 2, scale / 2 / aspect};
}

void set_main_camera(camera *cam)
{
	s_main_camera = cam;
}

camera *get_main_camera()
{
	return s_main_camera;
}

void camera_create(camera *cam)
{
	camera_texture_create(&cam->main_texture, camera_get_render_size(cam));
	camera_texture_create(&cam->lighting_texture, camera_get_render_size(cam));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void camera_texture_create(camera_texture *cam_texture, const vec2 render_size)
{
	camera_texture_create_framebuffer(cam_texture);
	camera_texture_create_renderbuffer(cam_texture, render_size);
	camera_texture_create_color_texture(cam_texture, render_size);
	camera_texture_create_depth_texture(cam_texture, render_size);
}

void camera_texture_create_framebuffer(camera_texture *cam_texture)
{
	if (glIsFramebuffer(cam_texture->framebuffer))
		return;

	glGenFramebuffers(1, &cam_texture->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, cam_texture->framebuffer);

	if (glIsTexture(cam_texture->color_texture))
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cam_texture->color_texture, 0);

	if (glIsRenderbuffer(cam_texture->renderbuffer))
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, cam_texture->renderbuffer);
}

void camera_texture_create_renderbuffer(camera_texture *cam_texture, const vec2 render_size)
{
	if (glIsRenderbuffer(cam_texture->color_texture))
		glDeleteRenderbuffers(1, &cam_texture->color_texture);

	glGenRenderbuffers(1, &cam_texture->renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, cam_texture->renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, render_size.x, render_size.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glIsFramebuffer(cam_texture->framebuffer))
	{
		glBindFramebuffer(GL_FRAMEBUFFER, cam_texture->framebuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, cam_texture->renderbuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void camera_texture_create_color_texture(camera_texture *cam_texture, const vec2 render_size)
{
	if (glIsTexture(cam_texture->color_texture))
		glDeleteTextures(1, &cam_texture->color_texture);

	glGenTextures(1, &cam_texture->color_texture);

	glBindTexture(GL_TEXTURE_2D, cam_texture->color_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, render_size.x, render_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glIsFramebuffer(cam_texture->framebuffer))
	{
		glBindFramebuffer(GL_FRAMEBUFFER, cam_texture->framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cam_texture->color_texture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void camera_texture_create_depth_texture(camera_texture *cam_texture, const vec2 render_size)
{
	if (glIsTexture(cam_texture->depth_texture))
		glDeleteTextures(1, &cam_texture->depth_texture);

	glGenTextures(1, &cam_texture->depth_texture);

	glBindTexture(GL_TEXTURE_2D, cam_texture->depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, render_size.x, render_size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glIsFramebuffer(cam_texture->framebuffer))
	{
		glBindFramebuffer(GL_FRAMEBUFFER, cam_texture->framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, cam_texture->depth_texture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void camera_destroy(const camera *cam)
{
	camera_texture_free(&cam->main_texture);
	camera_texture_free(&cam->lighting_texture);
}

void camera_texture_free(const camera_texture *cam_texture)
{
	camera_texture_free_color_texture(cam_texture);
	camera_texture_free_depth_texture(cam_texture);
	camera_texture_free_renderbuffer(cam_texture);
	camera_texture_free_framebuffer(cam_texture);
}

void camera_texture_free_framebuffer(const camera_texture *cam_texture)
{
	if (!glIsFramebuffer(cam_texture->framebuffer))
		return;
	glDeleteFramebuffers(1, &cam_texture->framebuffer);
}

void camera_texture_free_renderbuffer(const camera_texture *cam_texture)
{
	if (!glIsRenderbuffer(cam_texture->renderbuffer))
		return;
	glDeleteRenderbuffers(1, &cam_texture->renderbuffer);
}

void camera_texture_free_color_texture(const camera_texture *cam_texture)
{
	if (!glIsTexture(cam_texture->color_texture))
		return;
	glDeleteTextures(1, &cam_texture->color_texture);
}

void camera_texture_free_depth_texture(const camera_texture *cam_texture)
{
	if (!glIsTexture(cam_texture->depth_texture))
		return;
	glDeleteTextures(1, &cam_texture->depth_texture);
}

vec2 camera_get_render_size(const camera *cam)
{
	float aspect = window_get_aspect_ratio(gfx_get_window());
	aspect = SDL_clamp(aspect, MIN_ASPECT_RATIO, MAX_ASPECT_RATIO);
	return (vec2){cam->render_size, cam->render_size / aspect};
}

mat3 world_to_camera_matrix(const camera *cam)
{
	return mat3_translate(mat3_from_rotation(-cam->rotation), vec2_neg(cam->position));
}

mat3 camera_to_world_matrix(const camera *cam)
{
	return mat3_rotate(mat3_from_translation(cam->position), cam->rotation);
}

mat3 camera_to_screen_matrix(const camera *cam)
{
	return mat3_from_scale(get_matrix_scale(cam->size));
}

mat3 screen_to_camera_matrix(const camera *cam)
{
	return mat3_from_scale(get_matrix_scale_inverted(cam->size));
}

mat3 world_to_screen_matrix(const camera *cam)
{
	return mat3_multiply(camera_to_screen_matrix(cam), world_to_camera_matrix(cam));
}

mat3 screen_to_world_matrix(const camera *cam)
{
	return mat3_multiply(camera_to_world_matrix(cam), screen_to_camera_matrix(cam));
}

void camera_bind_main_framebuffer(const camera* cam)
{
	const vec2 size = camera_get_render_size(cam);

	glBindFramebuffer(GL_FRAMEBUFFER, cam->main_texture.framebuffer);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, size.x, size.y);
}

void camera_bind_lighting_framebuffer(const camera* cam)
{
	const vec2 size = camera_get_render_size(cam);

	glBindFramebuffer(GL_FRAMEBUFFER, cam->lighting_texture.framebuffer);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, size.x, size.y);
}

void camera_unbind_framebuffer(void)
{
	int width, height;
	window_get_size(gfx_get_window(), &width, &height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);
}

void camera_render_to_screen(const camera *cam)
{
	shader_set(s_shader);

	texture_set(cam->main_texture.color_texture, 0);
	shader_set_int32_t(s_shader, "main_texture", 0);

	texture_set(cam->lighting_texture.color_texture, 1);
	shader_set_int32_t(s_shader, "lighting_texture", 1);

	shader_set_vec2(s_shader, "resolution", camera_get_render_size(cam));

	glBindVertexArray(s_vertex_array);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

void camera_get_bounds(const camera *cam, vec2 *bounds_min, vec2 *bounds_max)
{
	const mat3 m = screen_to_world_matrix(cam);

	const vec2 a = vec2_transform((vec2){-1, 1}, m);
	const vec2 b = vec2_transform((vec2){-1, -1}, m);
	const vec2 c = vec2_transform((vec2){1, -1}, m);
	const vec2 d = vec2_transform((vec2){1, 1}, m);

	*bounds_min = (vec2){fminf(fminf(a.x, b.x), fminf(c.x, d.x)), fminf(fminf(a.y, b.y), fminf(c.y, d.y))};
	*bounds_max = (vec2){fmaxf(fmaxf(a.x, b.x), fmaxf(c.x, d.x)), fmaxf(fmaxf(a.y, b.y), fmaxf(c.y, d.y))};
}

void camera_window_resize(const camera *cam)
{
	const vec2 size = camera_get_render_size(cam);

	camera_texture_create_renderbuffer(&get_main_camera()->main_texture, size);
	camera_texture_create_color_texture(&get_main_camera()->main_texture, size);
	camera_texture_create_depth_texture(&get_main_camera()->main_texture, size);

	camera_texture_create_renderbuffer(&get_main_camera()->lighting_texture, size);
	camera_texture_create_color_texture(&get_main_camera()->lighting_texture, size);
	camera_texture_create_depth_texture(&get_main_camera()->lighting_texture, size);

	const float aspect = window_get_aspect_ratio(gfx_get_window());
	vec2 vertex_scale = VEC2_ONE;
	if (aspect < MIN_ASPECT_RATIO)
		vertex_scale.y = aspect / MIN_ASPECT_RATIO;
	else if (aspect > MAX_ASPECT_RATIO)
		vertex_scale.x = MAX_ASPECT_RATIO / aspect;

	for (int i = 0; i < sizeof(s_vertices) / sizeof(s_vertices[0]); i++)
	{
		s_vertices[i].position.x /= fabsf(s_vertices[i].position.x);
		s_vertices[i].position.y /= fabsf(s_vertices[i].position.y);
		s_vertices[i].position = vec2_scale(s_vertices[i].position, vertex_scale);
	}

	glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(s_vertices), s_vertices);
}