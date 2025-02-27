#include "globals/consts.h"
#include "globals/kernel.h"

#include "serial.h"
#include "libc.h"
#include "screen.h"

static volatile u8* const FRAMEBUFFER = (volatile u8* const)0xA0000;
static u8* const BACKBUFFER = (u8* const)(BACKBUFFER_BASE);

void InitializeScreen(void)
{
	DBG("Initializing screen, framebuffer at 0x%X and backbuffer at 0x%X", (uptr)FRAMEBUFFER, (uptr)BACKBUFFER);
	ClearScreen(0);
	FlipScreen();
}

void FlipScreen(void)
{
	memcpy((u8*)FRAMEBUFFER, BACKBUFFER, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void ClearScreen(u8 color)
{
	memset(BACKBUFFER, color, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void FixPoint(Point2i_t* p)
{
	p->x = MAX(p->x, 0);
	p->y = MAX(p->y, 0);
	p->x = MIN(p->x, SCREEN_WIDTH - 1);
	p->y = MIN(p->y, SCREEN_HEIGHT - 1);
}

void RawSetPixel(const Point2i_t* p, u8 color)
{
	if (color != TRANSPARENT_COLOR)
	{
		BACKBUFFER[p->y * SCREEN_WIDTH + p->x] = color;
	}
}

void SetPixel(const Point2i_t* pi, u8 color)
{
	Point2i_t p = *pi;
	FixPoint(&p);
	RawSetPixel(&p, color);
}

void Fill(const Point2i_t* p1i, const Point2i_t* p2i, u8 color)
{
	Point2i_t p1 = *p1i;
	Point2i_t p2 = *p2i;

	FixPoint(&p1);
	FixPoint(&p2);
	if (p1.x > p2.x)
	{
		SWAP(p1.x, p2.x);
	}
	if (p1.y > p2.y)
	{
		SWAP(p1.y, p2.y);
	}

	if (p1.x == p2.x)
	{
		for (s32 y = p1.y; y < p2.y; y++)
		{
			RawSetPixel(&P2I(p1.x, y), color);
		}
	}
	else
	{
		for (s32 y = p1.y; y < p2.y && y < SCREEN_HEIGHT; y++)
		{
			for (s32 x = p2.x; x < p2.x && x < SCREEN_WIDTH; x++)
			{
				RawSetPixel(&P2I(x, y), color);
			}
		}
	}
}

void DrawBitmap(const Point2i_t* p, const u8* bitmap, s32 width, s32 height)
{
	for (s32 curY = 0; curY < height; curY++)
	{
		for (s32 curX = 0; curX < width; curX++)
		{
			if (p->x + curX < SCREEN_WIDTH && p->y + curY < SCREEN_HEIGHT)
			{
				RawSetPixel(&P2I(p->x + curX, p->y + curY), bitmap[curY * width + curX]);
			}
		}
	}
}
