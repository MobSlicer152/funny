#pragma once

#include "kernel/macros.h"
#include "math.h"

typedef f32 Mat4_t[4][4];

#define M4I(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)                                      \
	(Mat4_t)                                                                                                                     \
	{                                                                                                                            \
		(f32[]){(m00), (m01), (m02), (m03)},                                                                                     \
		(f32[]){(m10), (m11), (m12), (m13)},                                                                                     \
		(f32[]){(m20), (m21), (m22), (m23)},                                                                                     \
		(f32[]){(m30), (m31), (m32), (m33)},                                                                                     \
	}

static FORCEINLINE void Mat4Mul(const Mat4_t* a, const Mat4_t* b)
{
}
