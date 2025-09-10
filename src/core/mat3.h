#pragma once

/* This is technically a 3x2 matrix which we'll treat as a 3x3 matrix.
 * 3x2 matrices can individually account for translation, rotation, and scaling
 * However, in order to do multiple transformations, we must multiply matrices.
 * To multiply matrices, the amount of rows in the first must equal the amount of columns in the second.
 * So, if all of our matrices represent the same amount of transformations, we'd need 3x3 matrices.
 * However, the last row of this matrix would just be [0, 0, 1] no matter what.
 * So, we don't need to actually store this row in each matrix because it's a constant.
*/

typedef struct mat3
{
	float m[2][3];
} mat3;

#define M_20 0
#define M_21 0
#define M_22 1

#define MAT3_IDENTITY (mat3){\
1, 0, 0,\
0, 1, 0 \
}

typedef struct vec2 vec2;

mat3 mat3_multiply(mat3 a, mat3 b);
mat3 mat3_multiplyf(mat3 m, float scalar);

float mat3_determinant(mat3 m);
mat3 mat3_inverse(mat3 m);

mat3 mat3_from_translation(vec2 translation);
mat3 mat3_from_rotation(float angle);
mat3 mat3_from_scale(vec2 scale);
mat3 mat3_from_trs(vec2 translation, float rotation, vec2 scale);

mat3 mat3_translate(mat3 m, vec2 translation);
mat3 mat3_rotate(mat3 m, float angle);
mat3 mat3_scale(mat3 m, vec2 scale);

mat3 mat3_from_ortho(float right, float left, float top, float bottom);