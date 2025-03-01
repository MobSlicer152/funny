#include "globals/consts.h"
#include "globals/memmap.h"

#include "libc.h"
#include "screen.h"
#include "serial.h"


static volatile u8* const FRAMEBUFFER = (volatile u8* const)0xA0000;
static u8* const BACKBUFFER = (u8* const)(BACKBUFFER_BASE);
static u8* const ZBUFFER = (u8* const)(ZBUFFER_BASE);

void InitializeScreen(void)
{
	DBG("Initializing screen, framebuffer at 0x%X and backbuffer at 0x%X", (uptr)FRAMEBUFFER, (uptr)BACKBUFFER);
	ClearScreen(0, 0);
	FlipScreen(false);
}

void FlipScreen(bool showZBuffer)
{
	if (showZBuffer)
	{
		// crunch [0..255] to [16..31] (utterly atrocious)
		for (u32 y = 0; y < SCREEN_HEIGHT; y++)
		{
			for (u32 x = 0; x < SCREEN_WIDTH; x++)
			{
				u8 v = 16 + ZBUFFER[y * SCREEN_WIDTH + x] / 16;
				FRAMEBUFFER[y * SCREEN_WIDTH + x] = v;
			}
		}
	}
	else
	{
		memcpy((u8*)FRAMEBUFFER, BACKBUFFER, SCREEN_WIDTH * SCREEN_HEIGHT);
	}
}

void ClearScreen(u8 color, u8 depth)
{
	memset(BACKBUFFER, color, SCREEN_WIDTH * SCREEN_HEIGHT);
	memset(ZBUFFER, depth, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void FixPoint(Vec2i_t p)
{
	p[0] = MAX(p[0], 0);
	p[1] = MAX(p[1], 0);
	p[0] = MIN(p[0], SCREEN_WIDTH - 1);
	p[1] = MIN(p[1], SCREEN_HEIGHT - 1);
}

void RawSetPixel(const Vec2i_t p, u8 color)
{
	if (color != TRANSPARENT_COLOR)
	{
		BACKBUFFER[p[1] * SCREEN_WIDTH + p[0]] = color;
	}
}

void WriteZBuffer(const Vec2i_t p, u8 value)
{
	ZBUFFER[p[1] * SCREEN_WIDTH + p[0]] = value;
}

u8 ReadZBuffer(const Vec2i_t p)
{
	return ZBUFFER[p[1] * SCREEN_WIDTH + p[0]];
}

void SetPixel(const Vec2i_t pi, u8 color)
{
	Vec2i_t p = V2I_DUP(pi);
	FixPoint(p);
	RawSetPixel(p, color);
}

void Fill(const Vec2i_t p1i, const Vec2i_t p2i, u8 color)
{
	Vec2i_t p1 = V2I_DUP(p1i);
	Vec2i_t p2 = V2I_DUP(p2i);

	FixPoint(p1);
	FixPoint(p2);
	if (p1[0] > p2[0])
	{
		SWAP(p1[0], p2[0]);
	}
	if (p1[1] > p2[1])
	{
		SWAP(p1[1], p2[1]);
	}

	if (p1[0] == p2[0])
	{
		for (s32 y = p1[1]; y < p2[1]; y++)
		{
			RawSetPixel(V2I(p1[0], y), color);
		}
	}
	else
	{
		for (s32 y = p1[1]; y < p2[1] && y < SCREEN_HEIGHT; y++)
		{
			for (s32 x = p2[0]; x < p2[0] && x < SCREEN_WIDTH; x++)
			{
				RawSetPixel(V2I(x, y), color);
			}
		}
	}
}

void DrawBitmap(const Vec2i_t p, const u8* bitmap, s32 width, s32 height)
{
	for (s32 curY = 0; curY < height; curY++)
	{
		for (s32 curX = 0; curX < width; curX++)
		{
			if (p[0] + curX < SCREEN_WIDTH && p[1] + curY < SCREEN_HEIGHT)
			{
				RawSetPixel(V2I(p[0] + curX, p[1] + curY), bitmap[curY * width + curX]);
			}
		}
	}
}
