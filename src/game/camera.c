#include "camera.h"

#include "gfx/window.h"
#include "core/vec2.h"
#include "core/mat3.h"

static vec2 get_scale(float scale)
{
	const float aspect = window_get_aspect_ratio();
	return (vec2){2 / scale / aspect, 2 / scale};
}

mat3 world_to_camera_matrix(const camera *cam)
{
	return mat3_translate(mat3_from_rotation(cam->rotation), cam->position);
}

mat3 camera_to_screen_matrix(const camera *cam)
{
	return mat3_from_scale(get_scale(cam->size));
}

mat3 world_to_screen_matrix(const camera *cam)
{
	// return mat3_from_trs(cam->position, cam->rotation, get_scale(cam->size));
	return mat3_multiply(camera_to_screen_matrix(cam), world_to_camera_matrix(cam));
}