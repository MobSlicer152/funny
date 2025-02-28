#pragma once

#include "kernel/types.h"
#include "math.h"

typedef s32 Vec2i_t[2];
typedef f32 Vec2f_t[2];

#define V2I(x, y)                                                                                                                \
	(Vec2i_t)                                                                                                                    \
	{                                                                                                                            \
		(x), (y)                                                                                                                 \
	}
#define V2F(x, y)                                                                                                                \
	(Vec2f_t)                                                                                                                    \
	{                                                                                                                            \
		(x), (y)                                                                                                                 \
	}
#define V2I_DUP(x)          V2I((x)[0], (x)[1])
#define V2F_DUP(x)          V2F((x)[0], (x)[1])
#define V2I_COPY(dest, src) memcpy((dest), (src), sizeof(Vec2i_t))
#define V2F_COPY(dest, src) memcpy((dest), (src), sizeof(Vec2f_t))

typedef s32 Vec3i_t[3];
typedef f32 Vec3f_t[3];

#define V3I(x, y, z)                                                                                                             \
	(Vec3i_t)                                                                                                                    \
	{                                                                                                                            \
		(x), (y), (z)                                                                                                            \
	}
#define V3F(x, y, z, w)                                                                                                          \
	(Vec3f_t)                                                                                                                    \
	{                                                                                                                            \
		(x), (y), (z)                                                                                                            \
	}
#define V3I_DUP(x)          V4I((x)[0], (x)[1], (x)[2])
#define V3F_DUP(x)          V4F((x)[0], (x)[1], (x)[2])
#define V3I_COPY(dest, src) memcpy((dest), (src), sizeof(Vec3i_t))
#define V3F_COPY(dest, src) memcpy((dest), (src), sizeof(Vec3f_t))

typedef s32 Vec4i_t[4];
typedef f32 Vec4f_t[4];

#define V4I(x, y, z, w)                                                                                                          \
	(Vec4i_t)                                                                                                                    \
	{                                                                                                                            \
		(x), (y), (z), (w)                                                                                                       \
	}
#define V4F(x, y, z, w)                                                                                                          \
	(Vec4f_t)                                                                                                                    \
	{                                                                                                                            \
		(x), (y), (z), (w)                                                                                                       \
	}
#define V4I_DUP(x)          V4I((x)[0], (x)[1], (x)[2], (x)[3])
#define V4F_DUP(x)          V4F((x)[0], (x)[1], (x)[2], (x)[3])
#define V4I_COPY(dest, src) memcpy((dest), (src), sizeof(Vec4i_t))
#define V4F_COPY(dest, src) memcpy((dest), (src), sizeof(Vec4f_t))
