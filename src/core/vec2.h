#pragma once
#include <stdbool.h>

typedef struct vec2
{
	float x;
	float y;
} vec2;

#define VEC2_ZERO (vec2){0, 0}
#define VEC2_ONE (vec2){1, 1}
#define VEC2_UNIT_X (vec2){1, 0}
#define VEC2_UNIT_Y (vec2){0, 1}

typedef struct mat3 mat3;

vec2 vec2_from_angle(float angle);

vec2 vec2_neg(vec2 v);
vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);
vec2 vec2_mul(vec2 v, float scalar);
vec2 vec2_div(vec2 v, float scalar);
vec2 vec2_scale(vec2 a, vec2 b);

vec2 vec2_transform(vec2 v, mat3 m);

float vec2_sqr_mag(vec2 v);
float vec2_mag(vec2 v);
vec2 vec2_set_mag(vec2 v, float magnitude);
vec2 vec2_clamp_mag(vec2 v, float max_magnitude);
vec2 vec2_normalize(vec2 v);

float vec2_angle(vec2 v);
float vec2_dot(vec2 a, vec2 b);
vec2 vec2_rotate(vec2 v, float angle);
vec2 vec2_project(vec2 a, vec2 b);
vec2 vec2_project_on_plane(vec2 a, vec2 normal);

bool vec2_equal(vec2 a, vec2 b);
bool vec2_is_zero(vec2 v);
bool vec2_is_normalized(vec2 v);