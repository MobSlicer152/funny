#include "serial.h"
#include "libc.h"
#include "screen.h"

static volatile u8* const FRAMEBUFFER = (volatile u8* const)0xA0000;
static u8* const BACKBUFFER = (u8* const)(BACKBUFFER_BASE);

void InitializeScreen(void)
{
	DebugPrint("Initializing screen\n");
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

void SetPixel(s32 x, s32 y, u8 color)
{
	FixPoint(&x, &y);
	BACKBUFFER[y * SCREEN_WIDTH + x] = color;
}

void Fill(s32 x1, s32 y1, s32 x2, s32 y2, u8 color)
{
	if (x1 > x2)
	{
		SWAP(x1, x2);
	}
	if (y1 > y2)
	{
		SWAP(y1, y2);
	}
	FixPoint(&x1, &y1);
	FixPoint(&x2, &y2);

	if (x1 == x2)
	{
		for (s32 y = y1; y < y2; y++)
		{
			BACKBUFFER[y * SCREEN_WIDTH + x1] = color;
		}
	}
	else
	{
		for (s32 y = y1; y < y2; y++)
		{
			memset(&BACKBUFFER[y * SCREEN_WIDTH + x1], color, x2 - x1);
		}
	}
}
