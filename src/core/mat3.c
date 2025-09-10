#include "mat3.h"
#include "vec2.h"

#include <math.h>

mat3 mat3_multiply(const mat3 a, const mat3 b)
{
	return (mat3)
	{
		//first row
		a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1] + a.m[2][0] * M_02,
		a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1] + a.m[2][0] * M_12,
		a.m[0][0] * b.m[2][0] + a.m[1][0] * b.m[2][1] + a.m[2][0] * M_22,

		//second row
		a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1] + a.m[2][1] * M_02,
		a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1] + a.m[2][1] * M_12,
		a.m[0][1] * b.m[2][0] + a.m[1][1] * b.m[2][1] + a.m[2][1] * M_22,
	};
}

mat3 mat3_multiplyf(const mat3 m, const float scalar)
{
	return (mat3)
	{
		m.m[0][0] * scalar, m.m[1][0] * scalar, m.m[2][0] * scalar,
		m.m[0][1] * scalar, m.m[1][1] * scalar, m.m[2][1] * scalar
	};
}

float mat3_determinant(const mat3 m)
{
	return m.m[0][0] * (m.m[1][1] * M_22 - m.m[2][1] * M_12)
	- m.m[1][0] * (m.m[0][1] * M_22 - m.m[2][1] * M_02)
	+ m.m[2][0] * (m.m[0][1] * M_12 - m.m[1][1] * M_02);
}

mat3 mat3_inverse(const mat3 m)
{
	return mat3_multiplyf((mat3)
	{
		//first row
		m.m[1][1] * M_22 - m.m[2][1] * M_12,
		m.m[2][0] * M_12 - m.m[1][0] * M_22,
		m.m[1][0] * m.m[2][1] - m.m[2][0] * m.m[1][1],

		//second row
		m.m[2][1] * M_02 - m.m[0][1] * M_22,
		m.m[2][0] * M_02 - m.m[0][0] * M_22,
		m.m[2][0] * m.m[0][1] - m.m[0][0] * m.m[2][1]
	}, 1 / mat3_determinant(m));
}

mat3 mat3_from_translation(const vec2 translation)
{
	return (mat3)
	{
		1, 0, translation.x,
		0, 1, translation.y
	};
}

mat3 mat3_from_rotation(const float angle)
{
	float s, c;
	sincosf(angle, &s, &c);

	return (mat3)
	{
		c, -s, 0,
		s, c, 0,
	};
}

mat3 mat3_from_scale(const vec2 scale)
{
	return (mat3)
	{
		scale.x, 0, 0,
		0, scale.y, 0
	};
}

mat3 mat3_from_trs(const vec2 translation, const float rotation, const vec2 scale)
{
	return mat3_translate(mat3_rotate(mat3_from_scale(scale), rotation), translation);
}

vec2 mat3_to_translation(const mat3 m)
{
	return (vec2){m.m[2][0], m.m[2][1]};
}

float mat3_to_rotation(const mat3 m)
{
	return atan2f(m.m[0][1], m.m[0][0]);
}

vec2 mat3_to_scale(const mat3 m)
{
	const float c = m.m[0][0];

	return vec2_div((vec2){m.m[0][0], m.m[1][1]}, c);
}

mat3 mat3_translate(const mat3 m, const vec2 translation)
{
	return mat3_multiply(m, mat3_from_translation(translation));
}

mat3 mat3_rotate(const mat3 m, const float angle)
{
	return mat3_multiply(m, mat3_from_rotation(angle));
}

mat3 mat3_scale(const mat3 m, const vec2 scale)
{
	return mat3_multiply(m, mat3_from_scale(scale));
}

mat3 mat3_from_ortho(const float right, const float left, const float top, const float bottom)
{
	return (mat3)
	{
		2.0f / (right - left), 0, (right + left) / (left - right),
		0, 2.0f / (top - bottom), (top + bottom) / (bottom - top)
	};
}