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
#define V2I_COPY(x) V2I((x)[0], (x)[1])
#define V2F_COPY(x) V2F((x)[0], (x)[1])

typedef s32 Vec3i_t[3];
typedef f32 Vec3f_t[3];

#define V3I(x, y, z)                                                                                                             \
	(Vec3i_t)                                                                                                                    \
	{                                                                                                                            \
		(x), (y), (z)                                                                                                            \
	}
#define V3F(x, y, z)                                                                                                             \
	(Vec3f_t)                                                                                                                    \
	{                                                                                                                            \
		(x), (y), (z)                                                                                                            \
	}
#define V3I_COPY(x) V3I((x)[0], (x)[1], (x)[2])
#define V3F_COPY(x) V3F((x)[0], (x)[1], (x)[2])
