#include "vec2.h"

#include <assert.h>
#include <math.h>

#include "mat3.h"

vec2 vec2_from_angle(const float angle)
{
	return (vec2){cos(angle), sin(angle)};
}

vec2 vec2_neg(const vec2 v)
{
	return (vec2){-v.x, -v.y};
}

vec2 vec2_add(const vec2 a, const vec2 b)
{
	return (vec2){a.x + b.x, a.y + b.y};
}

vec2 vec2_sub(const vec2 a, const vec2 b)
{
	return (vec2){a.x - b.x, a.y - b.y};
}

vec2 vec2_mul(const vec2 v, const float scalar)
{
	return (vec2){v.x * scalar, v.y * scalar};
}

vec2 vec2_div(const vec2 v, const float scalar)
{
	assert(scalar != 0);
	return (vec2){v.x / scalar, v.y / scalar};
}

vec2 vec2_scale(const vec2 a, const vec2 b)
{
	return (vec2){a.x * b.x, a.y * b.y};
}

vec2 vec2_transform(const vec2 v, const mat3 m)
{
	const vec2 v1 = vec2_mul((vec2){m.m[0][0], m.m[1][0]}, v.x);
	const vec2 v2 = vec2_mul((vec2){m.m[0][1], m.m[1][1]}, v.y);
	const vec2 v3 = (vec2){m.m[0][2], m.m[1][2]};

	return vec2_add(vec2_add(v1, v2), v3);
}

float vec2_sqr_mag(const vec2 v)
{
	return v.x * v.x + v.y * v.y;
}

float vec2_mag(const vec2 v)
{
	return sqrtf(vec2_sqr_mag(v));
}

vec2 vec2_set_mag(const vec2 v, const float magnitude)
{
	return vec2_mul(vec2_normalize(v), magnitude);
}

vec2 vec2_clamp_mag(const vec2 v, const float max_magnitude)
{
	return vec2_mul(vec2_normalize(v), fminf(max_magnitude, vec2_mag(v)));
}

vec2 vec2_normalize(const vec2 v)
{
	if (vec2_sqr_mag(v) == 0)
		return v;
	return vec2_div(v, vec2_mag(v));
}

float vec2_angle(const vec2 v)
{
	return atan2f(v.y, v.x);
}

float vec2_dot(const vec2 a, const vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

vec2 vec2_rotate(const vec2 v, float angle)
{
	angle += vec2_angle(v);
	return vec2_mul(vec2_from_angle(angle), vec2_mag(v));
}

vec2 vec2_project(const vec2 a, const vec2 b)
{
	float mag = vec2_dot(a, b);
	mag /= vec2_sqr_mag(b);
	return vec2_mul(b, mag);
}

vec2 vec2_project_on_plane(const vec2 a, vec2 normal)
{
	normal = vec2_normalize(normal);
	const float dot = vec2_dot(a, normal);
	if (dot > 0)
		return a;
	return vec2_sub(a, vec2_mul(normal, dot));
}

float vec2_angle_to(const vec2 a, const vec2 b)
{
	return vec2_angle(a) - vec2_angle(b);
}

bool vec2_equal(const vec2 a, const vec2 b)
{
	return a.x == b.x && a.y == b.y;
}

bool vec2_is_zero(const vec2 v)
{
	return v.x == 0 && v.y == 0;
}

bool vec2_is_normalized(const vec2 v)
{
	return vec2_sqr_mag(v) == 1.0f;
}