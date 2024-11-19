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

static void FixPoint(s32* x, s32* y)
{
	if (*x < 0)
	{
		*x = SCREEN_WIDTH + *x - 1;
	}
	if (*y < 0)
	{
		*y = SCREEN_HEIGHT + *y - 1;
	}
	if (*x >= SCREEN_WIDTH)
	{
		*x = SCREEN_WIDTH - 1;
	}
	if (*y >= SCREEN_HEIGHT)
	{
		*y = SCREEN_HEIGHT - 1;
	}
}

static void RawSetPixel(s32 x, s32 y, u8 color)
{
	if (color != TRANSPARENT_COLOR)
	{
		BACKBUFFER[y * SCREEN_WIDTH + x] = color;
	}
}

void SetPixel(s32 x, s32 y, u8 color)
{
	FixPoint(&x, &y);
	RawSetPixel(x, y, color);
}

void Fill(s32 x1, s32 y1, s32 x2, s32 y2, u8 color)
{
	FixPoint(&x1, &y1);
	FixPoint(&x2, &y2);
	if (x1 > x2)
	{
		SWAP(x1, x2);
	}
	if (y1 > y2)
	{
		SWAP(y1, y2);
	}

	if (x1 == x2)
	{
		for (s32 y = y1; y < y2; y++)
		{
			RawSetPixel(x1, y, color);
		}
	}
	else
	{
		for (s32 y = y1; y < y2 && y < SCREEN_HEIGHT; y++)
		{
			for (s32 x = x1; x < x2 && x < SCREEN_WIDTH; x++)
			{
				RawSetPixel(x, y, color);
			}
		}
	}
}

void DrawBitmap(s32 x, s32 y, const u8* bitmap, s32 width, s32 height)
{
	for (s32 curY = 0; curY < height && y + curY < SCREEN_HEIGHT; curY++)
	{
		for (s32 curX = 0; curX < width && x + curX < SCREEN_WIDTH; curX++)
		{
			RawSetPixel(x + curX, y + curY, bitmap[curY * width + curX]);
		}
	}
}
