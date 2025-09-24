#include <math.h>
#include "camera.h"

#include "gfx/window.h"
#include "core/vec2.h"
#include "core/mat3.h"
#include "gfx/gfx.h"
#include "gfx/shader.h"
#include "glad/glad.h"

static uint32_t s_vertex_array;
static uint32_t s_vertex_buffer;
static uint32_t s_index_buffer;
static shader s_shader;
static camera *s_main_camera;

typedef struct camera_quad_vertex
{
	vec2 position;
	vec2 tex_coord;
} camera_quad_vertex;

const camera_quad_vertex vertices[] =
{
	{-1, +1, +0, +1},
	{-1, -1, +0, +0},
	{+1, -1, +1, +0},
	{+1, +1, +1, +1}
};

void camera_init()
{
	glGenVertexArrays(1, &s_vertex_array);
	glBindVertexArray(s_vertex_array);

	glGenBuffers(1, &s_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(camera_quad_vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(camera_quad_vertex), (void*)offsetof(camera_quad_vertex, tex_coord));
	glEnableVertexAttribArray(1);

	const int indices[6] = QUAD_INDICES;
	glGenBuffers(1, &s_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	s_shader = shader_get("res/shaders/camera_render");
}

void camera_terminate()
{
	glDeleteVertexArrays(1, &s_vertex_array);
	glDeleteBuffers(1, &s_vertex_buffer);
	glDeleteBuffers(1, &s_index_buffer);
}

static vec2 get_matrix_scale(const float scale)
{
	const float aspect = window_get_aspect_ratio();
	return (vec2){2 / scale, 2 / scale * aspect};
}

static vec2 get_matrix_scale_inverted(const float scale)
{
	const float aspect = window_get_aspect_ratio();
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
	camera_create_framebuffer(cam);
	camera_create_texture(cam);
	camera_create_renderbuffer(cam);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void camera_create_framebuffer(camera *cam)
{
	if (glIsFramebuffer(cam->framebuffer))
		return;

	glGenFramebuffers(1, &cam->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, cam->framebuffer);

	if (glIsTexture(cam->texture))
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cam->texture, 0);

	if (glIsRenderbuffer(cam->renderbuffer))
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, cam->renderbuffer);
}

void camera_create_renderbuffer(camera *cam)
{
	if (glIsRenderbuffer(cam->texture))
		glDeleteRenderbuffers(1, &cam->texture);

	const vec2 size = camera_get_render_size(cam);

	glGenRenderbuffers(1, &cam->renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, cam->renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glIsFramebuffer(cam->framebuffer))
	{
		glBindFramebuffer(GL_FRAMEBUFFER, cam->framebuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, cam->renderbuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void camera_create_texture(camera *cam)
{
	if (glIsTexture(cam->texture))
		glDeleteTextures(1, &cam->texture);

	const vec2 size = camera_get_render_size(cam);
	glGenTextures(1, &cam->texture);

	glBindTexture(GL_TEXTURE_2D, cam->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glIsFramebuffer(cam->framebuffer))
	{
		glBindFramebuffer(GL_FRAMEBUFFER, cam->framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cam->texture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void camera_destroy(const camera *cam)
{
	camera_free_renderbuffer(cam);
	camera_free_texture(cam);
	camera_free_framebuffer(cam);
}

void camera_free_framebuffer(const camera *cam)
{
	if (!glIsFramebuffer(cam->texture))
		return;
	glDeleteFramebuffers(1, &cam->framebuffer);
}

void camera_free_renderbuffer(const camera *cam)
{
	if (!glIsRenderbuffer(cam->renderbuffer))
		return;
	glDeleteRenderbuffers(1, &cam->renderbuffer);
}

void camera_free_texture(const camera *cam)
{
	if (!glIsTexture(cam->texture))
		return;
	glDeleteTextures(1, &cam->texture);
}

vec2 camera_get_render_size(const camera *cam)
{
	const float aspect = window_get_aspect_ratio();
	return (vec2){cam->render_size, cam->render_size / aspect};
}

mat3 world_to_camera_matrix(const camera *cam)
{
	return mat3_translate(mat3_from_rotation(cam->rotation), vec2_neg(cam->position));
}

mat3 camera_to_world_matrix(const camera *cam)
{
	return mat3_rotate(mat3_from_translation(cam->position), -cam->rotation);
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

void camera_bind_framebuffer(const camera* cam)
{
	const vec2 size = camera_get_render_size(cam);

	glBindFramebuffer(GL_FRAMEBUFFER, cam->framebuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.4, 0.4, 0.2, 1.0f);
	glViewport(0, 0, size.x, size.y);
}

void camera_unbind_framebuffer(void)
{
	int width, height;
	window_get_size(&width, &height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.2, 0.4, 0.4, 1.0f);
	glViewport(0, 0, width, height);
}

void camera_render_to_screen(const camera *cam)
{
	shader_set(s_shader);
	shader_set_uint32_t(s_shader, "camera_texture", cam->texture);

	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(s_vertex_array);
	glBindTexture(GL_TEXTURE_2D, cam->texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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