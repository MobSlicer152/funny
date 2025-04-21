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
	return a[0] / w[0] + a[1] / w[1] + a[2] / w[2];
}

static FORCEINLINE void RenderPixel(
	const Vec3f_t p, const Vec3f_t w, const Vec3f_t tu, const Vec3f_t tv, const u8* texture, const u32 textureWidth,
	const u32 textureHeight)
{
	// divide by z
	f32 u = Interpolate(tu, w) * p[2];
	f32 v = Interpolate(tv, w) * p[2];

	// repeat the texture
	u = u - floor(u);
	v = v - floor(v);

	// scale to texture size
	u = (u * textureWidth);
	v = (v * textureHeight);

	// clamp within texture bounds
	u = MIN((u32)u, textureWidth - 1);
	v = MIN((u32)v, textureHeight - 1);

	RawSetPixel(V2I_DUP(p), texture[(u32)(v * textureWidth + u)]);

	WriteZBuffer(V2I_DUP(p), p[2]);
}

void DrawTriangle(const TriangleInfo_t* t)
{
	Vec2f_t min = V2F(MIN3(t->v0[0], t->v1[0], t->v2[0]), MIN3(t->v0[1], t->v1[1], t->v2[1]));
	Vec2f_t max = V2F(MAX3(t->v0[0], t->v1[0], t->v2[0]), MAX3(t->v0[1], t->v1[1], t->v2[1]));

	f32 areaDenom = 1.0f / Edge(t->v0, t->v1, t->v2);

	// this is where the edges are
	f32 e01r = Edge(t->v0, t->v1, min);
	f32 e12r = Edge(t->v1, t->v2, min);
	f32 e20r = Edge(t->v2, t->v0, min);

	// x steps for edges
	f32 xs01 = EdgeStep(t->v0, t->v1, V2F(1, 0));
	f32 xs12 = EdgeStep(t->v1, t->v2, V2F(1, 0));
	f32 xs20 = EdgeStep(t->v2, t->v0, V2F(1, 0));

	// y steps for edges
	f32 ys01 = EdgeStep(t->v0, t->v1, V2F(0, 1));
	f32 ys12 = EdgeStep(t->v1, t->v2, V2F(0, 1));
	f32 ys20 = EdgeStep(t->v2, t->v0, V2F(0, 1));

	for (f32 y = min[1]; y < max[1]; y++)
	{
		// current edge
		f32 e01 = e01r;
		f32 e12 = e12r;
		f32 e20 = e20r;
		for (f32 x = min[0]; x < max[0]; x++)
		{
			if (e01 > 0 && e12 > 0 && e20 > 0)
			{
				// current w
				Vec3f_t w;
				w[0] = e12 * areaDenom;
				w[1] = e20 * areaDenom;
				w[2] = e01 * areaDenom;

				// divide the texcoords by the zs
				Vec2f_t uv0 = V2F_DUP(t->t0);
				Vec2f_t uv1 = V2F_DUP(t->t1);
				Vec2f_t uv2 = V2F_DUP(t->t2);

				uv0[0] /= t->v0[2], uv0[1] /= t->v0[2];
				uv1[0] /= t->v1[2], uv1[1] /= t->v1[2];
				uv2[0] /= t->v2[2], uv2[1] /= t->v2[2];

				// interpolating between the un-inversed ones isn't allowed
				// z is the reciprocal
				f32 z = Interpolate(V3F(1.0f / t->v0[2], 1.0f / t->v1[2], 1.0f / t->v2[2]), w);
				if ((1.0f / z) < ReadZBuffer(V2I(x, y)))
				{
					RenderPixel(
						V3F(x, y, z), w, V3F(uv0[0], uv1[0], uv2[0]), V3F(uv0[1], uv1[1], uv2[1]), t->texture, t->tw, t->th);
				}
			}

			// step x
			e01 += xs01;
			e12 += xs12;
			e20 += xs20;
		}

		// step y
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

	const Face_t* faces = info->faces;

	// render
	for (s32 i = 0; i < info->faceCount; i++)
	{
		TriangleInfo_t t;

		// mvp the positions
		Mat4MulVec4(t.v0, mvp, info->faces[i][0].pos);
		Mat4MulVec4(t.v1, mvp, info->faces[i][1].pos);
		Mat4MulVec4(t.v2, mvp, info->faces[i][2].pos);

		// convert to screen coords/zbuffer values
		V3F_COPY(
			t.v0, V3F((t.v0[0] + 1.0f) * 0.5f * (SCREEN_WIDTH - 1), (t.v0[1] + 1.0f) * 0.5f * (SCREEN_HEIGHT - 1),
					  (t.v0[2] + 1.0f) * 0.5f));
		V3F_COPY(
			t.v1, V3F((t.v1[0] + 1.0f) * 0.5f * (SCREEN_WIDTH - 1), (t.v1[1] + 1.0f) * 0.5f * (SCREEN_HEIGHT - 1),
					  (t.v1[2] + 1.0f) * 0.5f));
		V3F_COPY(
			t.v2, V3F((t.v2[0] + 1.0f) * 0.5f * (SCREEN_WIDTH - 1), (t.v2[1] + 1.0f) * 0.5f * (SCREEN_HEIGHT - 1),
					  (t.v2[2] + 1.0f) * 0.5f));

		// copy the texcoords
		V2F_COPY(t.t0, faces[i][0].uv);
		V2F_COPY(t.t1, faces[i][1].uv);
		V2F_COPY(t.t2, faces[i][2].uv);

		// other texture parameters
		t.texture = info->texture;
		t.tw = info->textureWidth;
		t.th = info->textureHeight;

		// draw it
		DrawTriangle(&t);
	}
}
