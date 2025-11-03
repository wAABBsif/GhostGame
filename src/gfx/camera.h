#pragma once

#include <stdint.h>

#include "texture.h"
#include "core/vec2.h"

typedef struct camera_texture
{
	uint32_t framebuffer;
	uint32_t renderbuffer;
	texture_id color_texture;
	texture_id depth_texture;
} camera_texture;

typedef struct camera
{
	vec2 position;
	float rotation;
	float size;
	uint32_t render_size;
	camera_texture main_texture;
	camera_texture lighting_texture;
} camera;

void camera_init(void);
void camera_terminate(void);

void set_main_camera(camera *cam);
camera *get_main_camera(void);

void camera_create(camera *cam);
void camera_destroy(const camera *cam);

void camera_texture_create(camera_texture *cam_texture, vec2 render_size);
void camera_texture_create_framebuffer(camera_texture *cam_texture);
void camera_texture_create_renderbuffer(camera_texture *cam_texture, vec2 render_size);
void camera_texture_create_color_texture(camera_texture *cam_texture, vec2 render_size);
void camera_texture_create_depth_texture(camera_texture *cam_texture, vec2 render_size);

void camera_texture_free(const camera_texture *cam_texture);
void camera_texture_free_framebuffer(const camera_texture *cam_texture);
void camera_texture_free_renderbuffer(const camera_texture *cam_texture);
void camera_texture_free_color_texture(const camera_texture *cam_texture);
void camera_texture_free_depth_texture(const camera_texture *cam_texture);

vec2 camera_get_render_size(const camera *cam);

mat3 world_to_camera_matrix(const camera *cam);
mat3 camera_to_world_matrix(const camera *cam);
mat3 camera_to_screen_matrix(const camera *cam);
mat3 screen_to_camera_matrix(const camera *cam);
mat3 world_to_screen_matrix(const camera *cam);
mat3 screen_to_world_matrix(const camera *cam);

void camera_bind_main_framebuffer(const camera* cam);
void camera_bind_lighting_framebuffer(const camera* cam);
void camera_unbind_framebuffer(void);
void camera_render_to_screen(const camera *cam);

void camera_get_bounds(const camera *cam, vec2 *bounds_min, vec2 *bounds_max);