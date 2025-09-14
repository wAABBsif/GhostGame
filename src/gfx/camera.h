#pragma once

#include <stdint.h>

#include "core/vec2.h"

typedef struct camera
{
	vec2 position;
	float rotation;
	float size;
	uint32_t render_size;
	uint32_t framebuffer;
	uint32_t renderbuffer;
	uint32_t texture;
} camera;

void camera_init();
void camera_terminate();

void set_main_camera(camera *cam);
camera *get_main_camera();

void camera_create(camera *cam);
void camera_create_framebuffer(camera *cam);
void camera_create_renderbuffer(camera *cam);
void camera_create_texture(camera *cam);

void camera_destroy(const camera *cam);
void camera_free_framebuffer(const camera *cam);
void camera_free_renderbuffer(const camera *cam);
void camera_free_texture(const camera *cam);

vec2 camera_get_render_size(const camera *cam);
mat3 world_to_camera_matrix(const camera *cam);
mat3 camera_to_screen_matrix(const camera *cam);
mat3 world_to_screen_matrix(const camera *cam);

void camera_bind_framebuffer(const camera* cam);
void camera_unbind_framebuffer(void);
void camera_render_to_screen(const camera *cam);