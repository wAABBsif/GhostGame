#pragma once

#include "core/vec2.h"

typedef struct camera
{
	vec2 position;
	float rotation;
	float size;
} camera;

mat3 world_to_camera_matrix(const camera *cam);
mat3 camera_to_screen_matrix(const camera *cam);
mat3 world_to_screen_matrix(const camera *cam);