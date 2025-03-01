#include "base/heap.h"
#include "base/screen.h"
#include "base/serial.h"
#include "globals/consts.h"

#include "raster.h"

static f32 Edge(const Vec2f_t a, const Vec2f_t b, const Vec2f_t c)
{
	return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
}

static void RenderPixel(
	const Vec3i_t p, const Vec3f_t w, const Vec2f_t t0, const Vec2f_t t1, const Vec2f_t t2, const u8* texture,
	const u32 textureWidth, const u32 textureHeight)
{
	Vec2i_t tp;
	tp[0] = CLAMP((w[0] * t0[0] + w[1] * t1[0] + w[2] * t2[0]), 0.0f, 1.0f) * (textureWidth - 1);
	tp[1] = CLAMP((w[0] * t0[1] + w[1] * t1[1] + w[2] * t2[1]), 0.0f, 1.0f) * (textureHeight - 1);
	//DBG("(%d, %d)", tp[0], tp[1]);
	RawSetPixel(p, texture[(u32)(tp[1] * textureWidth + tp[0])]);
	WriteZBuffer(p, p[2]);
}

void DrawTriangle(const TriangleInfo_t* t)
{
	// needed for proper interpolation with barycentric coords
	Vec2f_t v0 = V2F((f32)t->v0[0] / SCREEN_WIDTH, (f32)t->v0[1] / SCREEN_HEIGHT);
	Vec2f_t v1 = V2F((f32)t->v1[0] / SCREEN_WIDTH, (f32)t->v1[1] / SCREEN_HEIGHT);
	Vec2f_t v2 = V2F((f32)t->v2[0] / SCREEN_WIDTH, (f32)t->v2[1] / SCREEN_HEIGHT);

	Vec2f_t min = {MIN3(v0[0], v1[0], v2[0]), MIN3(v0[1], v1[1], v2[1])};
	Vec2f_t max = {MAX3(v0[0], v1[0], v2[0]), MAX3(v0[1], v1[1], v2[1])};

	min[0] = MAX(min[0], 0);
	min[1] = MAX(min[1], 0);
	max[0] = MIN(max[0], 1);
	max[1] = MIN(max[1], 1);

	f32 bias0 = 0.0f;
	f32 bias1 = 0.0f;
	f32 bias2 = 0.0f;

	f32 denom = 1 / Edge(v0, v1, v2);
	if (denom < FLT_EPSILON)
	{
		return;
	}

	f32 a01 = v0[1] - v1[1];
	f32 a12 = v1[1] - v2[1];
	f32 a20 = v2[1] - v0[1];

	f32 b10 = v1[0] - v0[0];
	f32 b21 = v2[0] - v1[0];
	f32 b02 = v0[0] - v2[0];

	f32 w0r = Edge(v1, v2, min) * denom + bias0;
	f32 w1r = Edge(v2, v0, min) * denom + bias1;
	f32 w2r = Edge(v0, v1, min) * denom + bias2;

	Vec3i_t p;
	for (p[1] = min[1] * SCREEN_HEIGHT; p[1] < max[1] * SCREEN_HEIGHT; p[1]++)
	{
		f32 w0 = w0r;
		f32 w1 = w1r;
		f32 w2 = w2r;
		for (p[0] = min[0] * SCREEN_WIDTH; p[0] < max[0] * SCREEN_WIDTH; p[0]++)
		{
			// find the depth of this point
			p[2] = w0 * t->v0[2] + w1 * t->v1[2]  + w2 * t->v2[2];

			if (p[2] < ReadZBuffer(p) && w0 > FLT_EPSILON  && w1 > FLT_EPSILON && w2 > FLT_EPSILON)
			{
				RenderPixel(p, V3F(w0, w1, w2), t->t0, t->t1, t->t2, t->texture, t->tw, t->th);
			}

			w0 += a12;
			w1 += a20;
			w2 += a01;
		}

		w0r += b21;
		w1r += b02;
		w2r += b10;
	}
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
		V3I_COPY(
			t.v0, V3I((faces[i][0][0] + 1.0f) * 0.5f * SCREEN_WIDTH, (faces[i][0][1] + 1.0f) * 0.5f * SCREEN_HEIGHT,
					  faces[i][0][2] * 255));
		V3I_COPY(
			t.v1, V3I((faces[i][1][0] + 1.0f) * 0.5f * SCREEN_WIDTH, (faces[i][1][1] + 1.0f) * 0.5f * SCREEN_HEIGHT,
					  faces[i][1][2] * 255));
		V3I_COPY(
			t.v2, V3I((faces[i][2][0] + 1.0f) * 0.5f * SCREEN_WIDTH, (faces[i][2][1] + 1.0f) * 0.5f * SCREEN_HEIGHT,
					  faces[i][2][2] * 255));

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
