#pragma once

#include "kernel/base/libc.h"
#include "kernel/types.h"
#include "macros.h"
#include "math.h"

typedef s32 Vec2i_t[2];
typedef f32 Vec2f_t[2];

#define V2I(x, y)                                                                                                                \
	(Vec2i_t)                                                                                                                    \
	{                                                                                                                            \
		(s32)(x), (s32)(y)                                                                                                       \
	}
#define V2F(x, y)                                                                                                                \
	(Vec2f_t)                                                                                                                    \
	{                                                                                                                            \
		(f32)(x), (f32)(y)                                                                                                       \
	}
#define V2I_DUP(x)          V2I((x)[0], (x)[1])
#define V2F_DUP(x)          V2F((x)[0], (x)[1])
#define V2I_COPY(dest, src) memcpy((dest), (src), sizeof(Vec2i_t))
#define V2F_COPY(dest, src) memcpy((dest), (src), sizeof(Vec2f_t))

static FORCEINLINE void Vec2fAdd(Vec2f_t result, const Vec2f_t a, const Vec2f_t b)
{
	// a0 + b0
	// a1 + b1
	V2F_COPY(result, V2F(a[0] + b[0], a[1] + b[1]));
}

static FORCEINLINE void Vec2fSub(Vec2f_t result, const Vec2f_t a, const Vec2f_t b)
{
	// a0 - b0
	// a1 - b1
	V2F_COPY(result, V2F(a[0] - b[0], a[1] - b[1]));
}

static FORCEINLINE void Vec2fScale(Vec2f_t result, const Vec2f_t a, f32 b)
{
	// a0 * b
	// a1 * b
	V2F_COPY(result, V2F(a[0] * b, a[1] * b));
}

static FORCEINLINE f32 Vec2fDot(const Vec2f_t a, const Vec2f_t b)
{
	// a0 * b0 + a1 * b1
	return a[0] * b[0] + a[1] * b[1];
}

static FORCEINLINE f32 Vec2fLength(const Vec2f_t v)
{
	// sqrt(v0^2 + v1^2 + v2^2 + v3^2)
	return sqrt(v[0] * v[0] + v[1] * v[1]);
}

static FORCEINLINE void Vec2fNormalize(Vec2f_t result, const Vec2f_t v)
{
	f32 length = Vec2fLength(v);
	// v^ = v/|v|
	Vec2fScale(result, v, 1.0f / length);
}

typedef s32 Vec3i_t[3];
typedef f32 Vec3f_t[3];

#define V3I(x, y, z)                                                                                                             \
	(Vec3i_t)                                                                                                                    \
	{                                                                                                                            \
		(s32)(x), (s32)(y), (s32)(z)                                                                                             \
	}
#define V3F(x, y, z)                                                                                                             \
	(Vec3f_t)                                                                                                                    \
	{                                                                                                                            \
		(f32)(x), (f32)(y), (f32)(z)                                                                                             \
	}
#define V3I_DUP(x)          V3I((x)[0], (x)[1], (x)[2])
#define V3F_DUP(x)          V3F((x)[0], (x)[1], (x)[2])
#define V3I_COPY(dest, src) memcpy((dest), (src), sizeof(Vec3i_t))
#define V3F_COPY(dest, src) memcpy((dest), (src), sizeof(Vec3f_t))

typedef s32 Vec4i_t[4];
typedef f32 Vec4f_t[4];

#define V4I(x, y, z, w)                                                                                                          \
	(Vec4i_t)                                                                                                                    \
	{                                                                                                                            \
		(s32)(x), (s32)(y), (s32)(z), (s32)(w)                                                                                   \
	}
#define V4F(x, y, z, w)                                                                                                          \
	(Vec4f_t)                                                                                                                    \
	{                                                                                                                            \
		(f32)(x), (f32)(y), (f32)(z), (f32)(w)                                                                                   \
	}
#define V4I_DUP(x)          V4I((x)[0], (x)[1], (x)[2], (x)[3])
#define V4F_DUP(x)          V4F((x)[0], (x)[1], (x)[2], (x)[3])
#define V4I_COPY(dest, src) memcpy((dest), (src), sizeof(Vec4i_t))
#define V4F_COPY(dest, src) memcpy((dest), (src), sizeof(Vec4f_t))

#define V4F_UP    V4F(0.0f, 1.0f, 0.0f, 1.0f)
#define V4F_DOWN  V4F(0.0f, -1.0f, 0.0f, 1.0f)
#define V4F_LEFT  V4F(-1.0f, 0.0f, 0.0f, 1.0f)
#define V4F_RIGHT V4F(1.0f, 0.0f, 0.0f, 1.0f)
#define V4F_BACK  V4F(0.0f, 0.0f, -1.0f, 1.0f)
#define V4F_FRONT V4F(0.0f, 0.0f, 1.0f, 1.0f)

static FORCEINLINE void Vec4fAdd(Vec4f_t result, const Vec4f_t a, const Vec4f_t b)
{
	// a0 + b0
	// a1 + b1
	// a2 + b2
	// a3 + b3
	V4F_COPY(result, V4F(a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]));
}

static FORCEINLINE void Vec4fSub(Vec4f_t result, const Vec4f_t a, const Vec4f_t b)
{
	// a0 - b0
	// a1 - b1
	// a2 - b2
	// a3 - b3
	V4F_COPY(result, V4F(a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]));
}

static FORCEINLINE void Vec4fScale(Vec4f_t result, const Vec4f_t a, f32 b)
{
	// a0 * b
	// a1 * b
	// a2 * b
	// a3 * b
	V4F_COPY(result, V4F(a[0] * b, a[1] * b, a[2] * b, a[3] * b));
}

static FORCEINLINE void Vec4fCross(Vec4f_t result, const Vec4f_t a, const Vec4f_t b)
{
	V4F_COPY(result, V4F(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0], 1.0f));
}

static FORCEINLINE f32 Vec4fDot(const Vec4f_t a, const Vec4f_t b)
{
	// a0 * b0 + a1 * b1 + a2 * b2 + a3 * b3
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

static FORCEINLINE f32 Vec4fLength(const Vec4f_t v)
{
	// sqrt(v0^2 + v1^2 + v2^2 + v3^2)
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
}

static FORCEINLINE void Vec4fNormalize(Vec4f_t result, const Vec4f_t v)
{
	f32 length = Vec4fLength(v);
	// v^ = v/|v|
	Vec4fScale(result, v, 1.0f / length);
}
