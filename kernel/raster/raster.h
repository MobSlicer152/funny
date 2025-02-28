#pragma once

#include "kernel/math/matrix.h"
#include "kernel/math/vector.h"
#include "kernel/types.h"

extern void DrawTriangle(const Vec2i_t v0, const Vec2i_t v1, const Vec2i_t v2, u8 color);

typedef Vec3i_t Face_t[3];

typedef struct DrawInfo
{
	const Vec4f_t* vertices;
	const Vec2f_t* texCoords;
	const Vec3f_t* normals;
	const Face_t* faces;
	s32 faceCount;
	const Mat4f_t model;
	const Mat4f_t view;
	const Mat4f_t project;
} DrawInfo_t;

#define DRAW_INFO(vt, t, n, f, fc, m, v, p)                                                                                      \
	(DrawInfo_t)                                                                                                                 \
	{                                                                                                                            \
		.vertices = (vt), .texCoords = (t), .normals = (n), .faces = (f), .faceCount = (fc), .model = M4F_DUP_INLINE(m),         \
		.view = M4F_DUP_INLINE(v), .project = M4F_DUP_INLINE(p)                                                                  \
	}

extern void DrawMesh(const DrawInfo_t* info);
