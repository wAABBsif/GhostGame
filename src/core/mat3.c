#include "mat3.h"

#include <assert.h>

#include "vec2.h"

#include <math.h>

#include "logging.h"

mat3 mat3_multiply(const mat3 a, const mat3 b)
{
	return (mat3)
	{
		//first row
		a.rows[0][0] * b.rows[0][0] + a.rows[0][1] * b.rows[1][0] + a.rows[0][2] * M_20,
		a.rows[0][0] * b.rows[0][1] + a.rows[0][1] * b.rows[1][1] + a.rows[0][2] * M_21,
		a.rows[0][0] * b.rows[0][2] + a.rows[0][1] * b.rows[1][2] + a.rows[0][2] * M_22,

		//second row
		a.rows[1][0] * b.rows[0][0] + a.rows[1][1] * b.rows[1][0] + a.rows[1][2] * M_20,
		a.rows[1][0] * b.rows[0][1] + a.rows[1][1] * b.rows[1][1] + a.rows[1][2] * M_21,
		a.rows[1][0] * b.rows[0][2] + a.rows[1][1] * b.rows[1][2] + a.rows[1][2] * M_22,
	};
}

mat3 mat3_multiplyf(const mat3 m, const float scalar)
{
	return (mat3)
	{
		m.rows[0][0] * scalar, m.rows[0][1] * scalar, m.rows[0][2] * scalar,
		m.rows[1][0] * scalar, m.rows[1][1] * scalar, m.rows[1][2] * scalar
	};
}

float mat3_determinant(const mat3 m)
{
	return m.rows[0][0] * (m.rows[1][1] * M_22 - m.rows[1][2] * M_21)
	- m.rows[1][0] * (m.rows[0][1] * M_22 - m.rows[1][2] * M_20)
	+ m.rows[0][2] * (m.rows[0][1] * M_21 - m.rows[1][1] * M_20);
}

mat3 mat3_inverse(const mat3 m)
{
	const float determinant = mat3_determinant(m);
	assert(determinant != 0);

	return mat3_multiplyf((mat3)
	{
		//first row
		m.rows[1][1] * M_22 - m.rows[1][2] * M_21,
		m.rows[0][2] * M_21 - m.rows[1][0] * M_22,
		m.rows[1][0] * m.rows[1][2] - m.rows[0][2] * m.rows[1][1],

		//second row
		m.rows[1][2] * M_20 - m.rows[0][1] * M_22,
		m.rows[0][2] * M_20 - m.rows[0][0] * M_22,
		m.rows[0][2] * m.rows[0][1] - m.rows[0][0] * m.rows[1][2]
	}, 1 / determinant);
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
	const float s = sinf(angle);
	const float c = cosf(angle);

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
	return mat3_scale(mat3_rotate(mat3_from_translation(translation), rotation), scale);
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
	assert(right != left);
	assert(top != bottom);

	return (mat3)
	{
		2.0f / (right - left), 0, (right + left) / (left - right),
		0, 2.0f / (top - bottom), (top + bottom) / (bottom - top)
	};
}