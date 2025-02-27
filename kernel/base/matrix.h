#pragma once

#include "kernel/macros.h"
#include "math.h"

typedef f32 Mat4_t[4][4];

#define M4I(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)                                      \
	(Mat4_t)                                                                                                                     \
	{                                                                                                                            \
		(f32[]){(m00), (m01), (m02), (m03)}, (f32[]){(m10), (m11), (m12), (m13)}, (f32[]){(m20), (m21), (m22), (m23)},           \
			(f32[]){(m30), (m31), (m32), (m33)},                                                                                 \
	}

static FORCEINLINE void Mat4Mul(Mat4_t r, const Mat4_t a, const Mat4_t b)
{
#define X(a, b, r, i, j)                                                                                                         \
	(r)[i][j] = (a)[i][0] * (b)[0][j] + (a)[i][1] * (b)[1][j] + (a)[i][2] * (b)[2][j] + (a)[i][3] * (b)[3][j];

	X(a, b, r, 0, 0)
	X(a, b, r, 0, 1)
	X(a, b, r, 0, 2)
	X(a, b, r, 0, 3)

	X(a, b, r, 1, 0)
	X(a, b, r, 1, 1)
	X(a, b, r, 1, 2)
	X(a, b, r, 1, 3)

	X(a, b, r, 2, 0)
	X(a, b, r, 2, 1)
	X(a, b, r, 2, 2)
	X(a, b, r, 2, 3)

	X(a, b, r, 3, 0)
	X(a, b, r, 3, 1)
	X(a, b, r, 3, 2)
	X(a, b, r, 3, 3)

#undef X
}
