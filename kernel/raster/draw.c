#include "base/heap.h"
#include "base/screen.h"
#include "base/serial.h"

#include "globals/consts.h"

#include "macros.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "raster.h"
#include "types.h"

static FORCEINLINE f32 Edge(const Vec2f_t v0, const Vec2f_t v1, const Vec2f_t v2)
{
	return (v2[0] - v0[0]) * (v1[1] - v0[1]) - (v2[1] - v0[1]) * (v1[0] - v0[0]);
}

static FORCEINLINE f32 EdgeStep(const Vec2f_t v0, const Vec2f_t v1, const Vec2f_t step)
{
	return Edge(v0, v1, step) - Edge(v0, v1, V2F(0, 0));
}

static FORCEINLINE f32 Interpolate(const Vec3f_t a, const Vec3f_t w)
{
	return a[0] * w[0] + a[1] * w[1] + a[2] * w[2];
}

static FORCEINLINE f32 InterpolatePersp(const Vec3f_t a, const Vec3f_t w, const Vec3f_t z)
{
	return Interpolate(V3F(a[0] / z[0], a[1] / z[1], a[2] / z[2]), w);
}

static FORCEINLINE void RenderPixel(
	const Vec3f_t p, const Vec3f_t z, const Vec3f_t w, const Vec3f_t tu, const Vec3f_t tv, const u8* texture,
	const u32 textureWidth, const u32 textureHeight)
{
	f32 u = InterpolatePersp(tu, w, z) * p[2];
	f32 v = InterpolatePersp(tv, w, z) * p[2];

	u = u - floor(u);
	v = v - floor(v);

	Vec2i_t tp;
	tp[0] = (u * textureWidth);
	tp[1] = (v * textureHeight);

	tp[0] = MIN((u32)tp[0], textureWidth - 1);
	tp[1] = MIN((u32)tp[1], textureHeight - 1);

	RawSetPixel(V2I_DUP(p), texture[(u32)(tp[1] * textureWidth + tp[0])]);

	WriteZBuffer(V2I_DUP(p), p[2]);
}

void DrawTriangle(const TriangleInfo_t* t)
{
	Vec2f_t min = V2F(MIN3(t->v0[0], t->v1[0], t->v2[0]), MIN3(t->v0[1], t->v1[1], t->v2[1]));
	Vec2f_t max = V2F(MAX3(t->v0[0], t->v1[0], t->v2[0]), MAX3(t->v0[1], t->v1[1], t->v2[1]));

	f32 areaDenom = 1.0f / Edge(t->v0, t->v1, t->v2);

	f32 e01r = Edge(t->v0, t->v1, min);
	f32 e12r = Edge(t->v1, t->v2, min);
	f32 e20r = Edge(t->v2, t->v0, min);

	f32 xs01 = EdgeStep(t->v0, t->v1, V2F(1, 0));
	f32 xs12 = EdgeStep(t->v1, t->v2, V2F(1, 0));
	f32 xs20 = EdgeStep(t->v2, t->v0, V2F(1, 0));

	f32 ys01 = EdgeStep(t->v0, t->v1, V2F(0, 1));
	f32 ys12 = EdgeStep(t->v1, t->v2, V2F(0, 1));
	f32 ys20 = EdgeStep(t->v2, t->v0, V2F(0, 1));

	for (f32 y = min[1]; y < max[1]; y++)
	{
		f32 e01 = e01r;
		f32 e12 = e12r;
		f32 e20 = e20r;
		for (f32 x = min[0]; x < max[0]; x++)
		{
			if (e01 > 0 && e12 > 0 && e20 > 0)
			{
				Vec3f_t w;
				w[0] = e12 * areaDenom;
				w[1] = e20 * areaDenom;
				w[2] = e01 * areaDenom;

				// interpolating between the un-inversed ones isn't allowed
				f32 z = 1.0f / Interpolate(V3F(1.0f / t->v0[2], 1.0f / t->v1[2], 1.0f / t->v2[2]), w);
				if (z < ReadZBuffer(V2I(x, y)))
				{
					RenderPixel(
						V3F(x, y, z), V3F(t->v0[2], t->v1[2], t->v2[2]), w, V3F(t->t0[0], t->t1[0], t->t2[0]),
						V3F(t->t0[1], t->t1[1], t->t2[1]), t->texture, t->tw, t->th);
				}
			}

			e01 += xs01;
			e12 += xs12;
			e20 += xs20;
		}

		e01r += ys01;
		e12r += ys12;
		e20r += ys20;
	}
}

void DrawMesh(const DrawInfo_t* info)
{
	Mat4f_t mvp;
	Mat4Mul(mvp, info->view, info->model);
	Mat4Mul(mvp, info->project, mvp);

	f32* rawFaces = HeapAlloc(
		GetKernelHeap(),
		info->faceCount * 3 * sizeof(Vec4f_t) + // 3 Vec4f vertices for each face
			info->faceCount * sizeof(f32*)      // plus a pointer to the start of each face
	);
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

	// TODO: sort faces

	// render
	for (s32 i = 0; i < info->faceCount; i++)
	{
		TriangleInfo_t t;

		// convert to screen coords/zbuffer values
		V3F_COPY(
			t.v0, V3F((faces[i][0][0] + 1.0f) * 0.5f * (SCREEN_WIDTH - 1), (faces[i][0][1] + 1.0f) * 0.5f * (SCREEN_HEIGHT - 1),
					  (faces[i][0][2] + 1.0f) * 0.5f));
		V3F_COPY(
			t.v1, V3F((faces[i][1][0] + 1.0f) * 0.5f * (SCREEN_WIDTH - 1), (faces[i][1][1] + 1.0f) * 0.5f * (SCREEN_HEIGHT - 1),
					  (faces[i][1][2] + 1.0f) * 0.5f));
		V3F_COPY(
			t.v2, V3F((faces[i][2][0] + 1.0f) * 0.5f * (SCREEN_WIDTH - 1), (faces[i][2][1] + 1.0f) * 0.5f * (SCREEN_HEIGHT - 1),
					  (faces[i][2][2] + 1.0f) * 0.5f));

		t.texture = info->texture;
		t.tw = info->textureWidth;
		t.th = info->textureHeight;

		DrawTriangle(&t);
	}

	HeapFree(GetKernelHeap(), rawFaces);
}
