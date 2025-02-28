#pragma once

#include "kernel/base/libc.h"
#include "kernel/macros.h"
#include "math.h"
#include "vector.h"

typedef Vec4f_t Mat4f_t[4];

#define M4F_INLINE(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)                               \
	{                                                                                                                            \
		{(m00), (m01), (m02), (m03)}, {(m10), (m11), (m12), (m13)}, {(m20), (m21), (m22), (m23)},                                \
		{                                                                                                                        \
			(m30), (m31), (m32), (m33)                                                                                           \
		}                                                                                                                        \
	}

// smh should be f4f
#define M4F(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)                                      \
	(Mat4f_t) M4F_INLINE(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)

#define M4F_DUP_INLINE(x)                                                                                                        \
	M4F_INLINE(                                                                                                                  \
		(x)[0][0], (x)[0][1], (x)[0][2], (x)[0][3], (x)[1][0], (x)[1][1], (x)[1][2], (x)[1][3], (x)[2][0], (x)[2][1], (x)[2][2], \
		(x)[2][3], (x)[3][0], (x)[3][1], (x)[3][2], (x)[3][3])
#define M4F_DUP(x)          (Mat4f_t) M4F_DUP_INLINE(x)
#define M4F_COPY(dest, src) memcpy((dest), (src), sizeof(Mat4f_t))

#define M4F_IDENTITY_INLINE M4F_INLINE(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
#define M4F_IDENTITY        (Mat4f_t) M4F_IDENTITY_INLINE
#define M4F_ZERO(m)         memset((m), 0, sizeof(Mat4f_t))

static FORCEINLINE void Mat4Mul(Mat4f_t result, const Mat4f_t a, const Mat4f_t b)
{
	Mat4f_t c = {};

	// c[i][j] = dot(a[i][], b[][j])

#define X(a, b, c, i, j) (c)[i][j] = (a)[i][0] * (b)[0][j] + (a)[i][1] * (b)[1][j] + (a)[i][2] * (b)[2][j];
	X(a, b, c, 0, 0)
	X(a, b, c, 0, 1)
	X(a, b, c, 0, 2)
	X(a, b, c, 0, 3)

	X(a, b, c, 1, 0)
	X(a, b, c, 1, 1)
	X(a, b, c, 1, 2)
	X(a, b, c, 1, 3)

	X(a, b, c, 2, 0)
	X(a, b, c, 2, 1)
	X(a, b, c, 2, 2)
	X(a, b, c, 2, 3)

	X(a, b, c, 3, 0)
	X(a, b, c, 3, 1)
	X(a, b, c, 3, 2)
	X(a, b, c, 3, 3)
#undef X

	M4F_COPY(result, c);
}

static FORCEINLINE void Mat4MulVec4(Vec4f_t result, const Mat4f_t a, const Vec4f_t b)
{
	Vec4f_t c = {};

#define X(a, b, c, i) (c)[i] = (a)[i][0] * (b)[0] + (a)[i][1] * (b)[1] + (a)[i][2] * (b)[2];
	X(a, b, c, 0)
	X(a, b, c, 1)
	X(a, b, c, 2)
	X(a, b, c, 3)
#undef X

	V4F_COPY(result, c);
}

static FORCEINLINE void Mat4Scale(Mat4f_t result, f32 x, f32 y, f32 z)
{
	// x 0 0 0
	// 0 y 0 0
	// 0 0 z 0
	// 0 0 0 1
	M4F_COPY(result, M4F(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1));
}

static FORCEINLINE void Mat4RotateX(Mat4f_t result, f32 angle)
{
	f32 t = angle;
	f32 st = sin(t);
	f32 ct = cos(t);
	//    1    0       0     0
	//    0  cos(t) -sin(t)  0
	//    0  sin(t)  cos(t)  0
	//    0    0       0     1
	M4F_COPY(result, M4F(1, 0, 0, 0, 0, ct, -st, 0, 0, st, ct, 0, 0, 0, 0, 1));
}

static FORCEINLINE void Mat4RotateY(Mat4f_t result, f32 angle)
{
	f32 t = angle;
	f32 st = sin(t);
	f32 ct = cos(t);
	//  cos(t)  0  sin(t)  0
	//    0     1    0     0
	// -sin(t)  0  cos(t)  0
	M4F_COPY(result, M4F(ct, 0, st, 0, 0, 1, 0, 0, -st, 0, ct, 0, 0, 0, 0, 1));
}

static FORCEINLINE void Mat4RotateZ(Mat4f_t result, f32 angle)
{
	f32 t = angle;
	f32 st = sin(t);
	f32 ct = cos(t);
	//  cos(t) -sin(t)   0     0
	//  sin(t)  cos(t)   0     0
	//    0       0      1     0
	//    0       0      0     1
	M4F_COPY(result, M4F(ct, -st, 0, 0, st, ct, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
}

static FORCEINLINE void Mat4Rotate(Mat4f_t result, f32 x, f32 y, f32 z)
{
	f32 a = x;
	f32 ca = cos(a);
	f32 sa = sin(a);

	f32 b = y;
	f32 cb = cos(b);
	f32 sb = sin(b);

	f32 g = z;
	f32 cg = cos(g);
	f32 sg = sin(g);

	//      cb*cg           -cb*sg         sb    0
	//  sa*sb*cg+ca*sg  -sa*sb*sg+ca*cg  -sa*cb  0
	// -ca*sb*cg+sa*sg   ca*sb*sg+sa*cg   ca*cb  0
	//        0               0             0    1
	M4F_COPY(
		result, M4F(cb * cg, -cb * sg, sb, 0, sa * sb * cg + ca * sg, -sa * sb * sg + ca * cg, -sa * cb, 0,
					-ca * sb * cg + sa * sg, ca * sb * sg + sa * cg, ca * cb, 0, 0, 0, 0, 1));
}

static FORCEINLINE void Mat4Translate(Mat4f_t result, f32 x, f32 y, f32 z)
{
	// 1 0 0 x
	// 0 1 0 y
	// 0 0 1 z
	// 0 0 0 1
	M4F_COPY(result, M4F(1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1));
}
