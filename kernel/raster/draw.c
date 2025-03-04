#include "base/heap.h"
#include "base/screen.h"
#include "base/serial.h"
#include "globals/consts.h"

#include "raster.h"

static void RenderPixel(
	const Vec3i_t p, const Vec3f_t w, const Vec2f_t t0, const Vec2f_t t1, const Vec2f_t t2, const u8* texture,
	const u32 textureWidth, const u32 textureHeight)
{
	Vec2i_t tp;
	tp[0] = CLAMP((w[0] * t0[0] + w[1] * t1[0] + w[2] * t2[0]), 0.0f, 1.0f) * (textureWidth - 1);
	tp[1] = CLAMP((w[0] * t0[1] + w[1] * t1[1] + w[2] * t2[1]), 0.0f, 1.0f) * (textureHeight - 1);
	// DBG("(%d, %d)", tp[0], tp[1]);
	RawSetPixel(p, texture[(u32)(tp[1] * textureWidth + tp[0])]);
	WriteZBuffer(p, p[2]);
}

void DrawTriangle(const TriangleInfo_t* t)
{
	
}

void DrawMesh(const DrawInfo_t* info)
{
	Mat4f_t mvp;
	Mat4Mul(mvp, info->view, info->model);
	Mat4Mul(mvp, info->project, mvp);

	f32* rawFaces = HeapAlloc(GetKernelHeap(), info->faceCount * 3 * sizeof(Vec4f_t) + info->faceCount * sizeof(f32*));
	Vec4f_t** faces = (Vec4f_t**)rawFaces;

	// convert faces to clip space
	for (s32 i = 0; i < info->faceCount; i++)
	{
		faces[i] = (Vec4f_t*)&((u8*)rawFaces)[info->faceCount * sizeof(f32*) + i * 3 * sizeof(Vec4f_t)];

		f32* v0 = faces[i][0];
		f32* v1 = faces[i][1];
		f32* v2 = faces[i][2];

		Mat4MulVec4(v0, mvp, info->vertices[info->faces[i][0][0]]);
		Mat4MulVec4(v1, mvp, info->vertices[info->faces[i][1][0]]);
		Mat4MulVec4(v2, mvp, info->vertices[info->faces[i][2][0]]);
	}

	// sort

	// render
	for (s32 i = 0; i < info->faceCount; i++)
	{
		TriangleInfo_t t;

		// convert to screen coords/zbuffer values
		V3F_COPY(t.v0, V3I((faces[i][0][0] + 1.0f) * 0.5f, (faces[i][0][1] + 1.0f) * 0.5f, faces[i][0][2]));
		V3F_COPY(t.v1, V3I((faces[i][1][0] + 1.0f) * 0.5f, (faces[i][1][1] + 1.0f) * 0.5f, faces[i][1][2]));
		V3F_COPY(t.v2, V3I((faces[i][2][0] + 1.0f) * 0.5f, (faces[i][2][1] + 1.0f) * 0.5f, faces[i][2][2]));

		V2F_COPY(t.t0, V2F_DUP(info->texCoords[info->faces[i][1][0]]));
		V2F_COPY(t.t1, V2F_DUP(info->texCoords[info->faces[i][1][1]]));
		V2F_COPY(t.t2, V2F_DUP(info->texCoords[info->faces[i][1][2]]));

		t.texture = info->texture;
		t.tw = info->textureWidth;
		t.th = info->textureHeight;

		DrawTriangle(&t);
	}

	HeapFree(GetKernelHeap(), rawFaces);
}
