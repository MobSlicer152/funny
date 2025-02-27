#include "base/screen.h"
#include "base/serial.h"


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
	RawSetPixel(p, color);
}

void DrawTriangle(const Vec2i_t v0, const Vec2i_t v1, const Vec2i_t v2, u8 color)
{
	Vec2i_t min = {MIN3(v0[0], v1[0], v2[0]), MIN3(v0[1], v1[1], v2[1])};
	Vec2i_t max = {MAX3(v0[0], v1[0], v2[0]), MAX3(v0[1], v1[1], v2[1])};

	FixPoint(min);
	FixPoint(max);

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
	for (p[1] = min[1]; p[1] <= max[1]; p[1]++)
	{
        s32 w0 = w0r;
        s32 w1 = w1r;
        s32 w2 = w2r;
		for (p[0] = min[0]; p[0] <= max[0]; p[0]++)
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
