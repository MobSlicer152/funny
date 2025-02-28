#pragma once

#include "kernel/base/libc.h"
#include "kernel/macros.h"
#include "math.h"
#include "vector.h"

typedef f32 Mat3_t[3][3];

#define M4I(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)                                      \
	(Mat4_t)                                                                                                                     \
	{                                                                                                                            \
		(f32[]){(m00), (m01), (m02), (m03)}, (f32[]){(m10), (m11), (m12), (m13)}, (f32[]){(m20), (m21), (m22), (m23)},           \
			(f32[]){(m30), (m31), (m32), (m33)},                                                                                 \
	}

static FORCEINLINE void Mat3Mul(Mat3_t result, const Mat3_t a, const Mat3_t b)
{
	Mat3_t c = {};

#define X(a, b, c, i, j) (c)[i][j] = (a)[i][0] * (b)[0][j] + (a)[i][1] * (b)[1][j] + (a)[i][2] * (b)[2][j];

	X(a, b, c, 0, 0)
	X(a, b, c, 0, 1)
	X(a, b, c, 0, 2)

	X(a, b, c, 1, 0)
	X(a, b, c, 1, 1)
	X(a, b, c, 1, 2)

	X(a, b, c, 2, 0)
	X(a, b, c, 2, 1)
	X(a, b, c, 2, 2)
#undef X

	memcpy(result, c, sizeof(Mat3_t));
}

static FORCEINLINE void Mat3MulVec3(Vec3f_t result, const Mat3_t a, const Vec3f_t b)
{
	Vec3f_t c = {};

#define X(a, b, c, i) (c)[i] = (a)[i][0] * (b)[0] + (a)[i][1] * (b)[1] + (a)[i][2] * (b)[2];

	X(a, b, c, 0)
	X(a, b, c, 1)
	X(a, b, c, 2)
#undef X

	memcpy(result, c, sizeof(Vec3f_t));
}
