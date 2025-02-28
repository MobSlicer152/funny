#include "base/screen.h"
#include "base/serial.h"
#include "globals/consts.h"

#include "raster.h"

static s32 Orient2(const Vec2i_t a, const Vec2i_t b, const Vec2i_t c)
{
	return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
}

static void RenderPixel(const Vec2i_t p, s32 w0, s32 w1, s32 w2, u8 color)
{
	UNUSED(w0);
	UNUSED(w1);
	UNUSED(w2);
	Vec2i_t pi = {(s32)round(p[0] * SCREEN_WIDTH), (s32)round(p[1] * SCREEN_HEIGHT)};
	RawSetPixel(pi, color);
}

void DrawTriangle(const Vec2i_t v0, const Vec2i_t v1, const Vec2i_t v2, u8 color)
{
	Vec2i_t min = {MIN3(v0[0], v1[0], v2[0]), MIN3(v0[1], v1[1], v2[1])};
	Vec2i_t max = {MAX3(v0[0], v1[0], v2[0]), MAX3(v0[1], v1[1], v2[1])};

	min[0] = MAX(min[0], 0);
	min[1] = MAX(min[1], 0);
	max[0] = MIN(max[0], SCREEN_WIDTH);
	max[1] = MIN(max[1], SCREEN_HEIGHT);

	s32 bias0 = 0;
	s32 bias1 = 0;
	s32 bias2 = 0;

	s32 a01 = v0[1] - v1[1];
	s32 a12 = v1[1] - v2[1];
	s32 a20 = v2[1] - v0[1];

	s32 b10 = v1[0] - v0[0];
	s32 b21 = v2[0] - v1[0];
	s32 b02 = v0[0] - v2[0];

	s32 w0r = Orient2(v1, v2, min) + bias0;
	s32 w1r = Orient2(v2, v0, min) + bias1;
	s32 w2r = Orient2(v0, v1, min) + bias2;

	Vec2i_t p;
	for (p[1] = min[1]; p[1] < max[1]; p[1]++)
	{
		s32 w0 = w0r;
		s32 w1 = w1r;
		s32 w2 = w2r;
		for (p[0] = min[0]; p[0] < max[0]; p[0]++)
		{
			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				RenderPixel(p, w0, w1, w2, color);
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

void DrawMesh(const Vec3f_t* vertices, const Vec3i_t* indices, const u8* colors, usize triangleCount)
{
	for (usize i = 0; i < triangleCount; i++)
	{
		const f32* v0f = vertices[indices[i][0]];
		const f32* v1f = vertices[indices[i][1]];
		const f32* v2f = vertices[indices[i][2]];

		Vec3i_t v0 = {v0f[0] * SCREEN_WIDTH, v0f[1] * SCREEN_HEIGHT, v0f[2]};
		Vec3i_t v1 = {v1f[0] * SCREEN_WIDTH, v1f[1] * SCREEN_HEIGHT, v1f[2]};
		Vec3i_t v2 = {v2f[0] * SCREEN_WIDTH, v2f[1] * SCREEN_HEIGHT, v2f[2]};

		DrawTriangle(v0, v1, v2, colors[i]);
	}
}
