#pragma once

#include "kernel/math/matrix.h"
#include "kernel/math/vector.h"
#include "kernel/types.h"

typedef struct TriangleInfo
{
	Vec3f_t v0;
	Vec3f_t v1;
	Vec3f_t v2;

	Vec2f_t t0;
	Vec2f_t t1;
	Vec2f_t t2;

	const u8* texture;
	u32 tw; // texture width
	u32 th; // texture height
} TriangleInfo_t;

extern void DrawTriangle(const TriangleInfo_t* t);

typedef struct Vertex
{
	Vec4f_t pos;
	Vec2f_t uv;
	Vec3f_t nrm;
} Vertex_t;
typedef Vertex_t Face_t[3];

typedef struct DrawInfo
{
	const Face_t* faces;
	s32 faceCount;

	const u8* texture;
	u32 textureWidth;
	u32 textureHeight;

	const Mat4f_t model;
	const Mat4f_t view;
	const Mat4f_t project;
} DrawInfo_t;

#define DRAW_INFO(f, fc, t, tw, th, m, v, p)                                                                                     \
	(DrawInfo_t)                                                                                                                 \
	{                                                                                                                            \
		.faces = (f), .faceCount = (fc), .texture = (t), .textureWidth = (tw), .textureHeight = (th),                            \
		.model = M4F_DUP_INLINE(m), .view = M4F_DUP_INLINE(v), .project = M4F_DUP_INLINE(p)                                      \
	}

extern void DrawMesh(const DrawInfo_t* info);
