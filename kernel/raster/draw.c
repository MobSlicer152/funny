#include "base/screen.h"
#include "base/serial.h"


#include "raster.h"

static s32 Orient2(const Point2i_t* a, const Point2i_t* b, const Point2i_t* c)
{
	return (b->x - a->x) * (c->y - a->y) - (b->y - a->y) * (c->x - a->x);
}

static void RenderPixel(const Point2i_t* p, s32 w0, s32 w1, s32 w2, u8 color)
{
	UNUSED(w0);
	UNUSED(w1);
	UNUSED(w2);
	RawSetPixel(p, color);
}

void DrawTriangle(const Point2i_t* v0, const Point2i_t* v1, const Point2i_t* v2, u8 color)
{
	Point2i_t min = P2I(MIN3(v0->x, v1->x, v2->x), MIN3(v0->y, v1->y, v2->y));
	Point2i_t max = P2I(MAX3(v0->x, v1->x, v2->x), MAX3(v0->y, v1->y, v2->y));

	FixPoint(&min);
	FixPoint(&max);

	s32 bias0 = 0;
	s32 bias1 = 0;
	s32 bias2 = 0;

	s32 a01 = v0->y - v1->y;
	s32 a12 = v1->y - v2->y;
	s32 a20 = v2->y - v0->y;

	s32 b10 = v1->x - v0->x;
	s32 b21 = v2->x - v1->x;
	s32 b02 = v0->x - v2->x;

	s32 w0r = Orient2(v1, v2, &min) + bias0;
	s32 w1r = Orient2(v2, v0, &min) + bias1;
	s32 w2r = Orient2(v0, v1, &min) + bias2;

	Point2i_t p;
	for (p.y = min.y; p.y <= max.y; p.y++)
	{
        s32 w0 = w0r;
        s32 w1 = w1r;
        s32 w2 = w2r;
		for (p.x = min.x; p.x <= max.x; p.x++)
		{
			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				RenderPixel(&p, w0, w1, w2, color);
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
