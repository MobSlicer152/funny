#include "screen.h"
#include "libc.h"
#include "math.h"

static volatile u8* const FRAMEBUFFER = (volatile u8* const)0xA0000;
static u8* const BACKBUFFER = (u8* const)(BACKBUFFER_BASE);

void InitializeScreen(void)
{
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

void SetPixel(s32 x, s32 y, u8 color)
{
	if (x < 0)
	{
		x = SCREEN_WIDTH + x - 1;
	}
	if (y < 0)
	{
		y = SCREEN_HEIGHT + y - 1;
	}
	if (x >= SCREEN_WIDTH)
	{
		x = SCREEN_WIDTH - 1;
	}
	if (y >= SCREEN_HEIGHT)
	{
		y = SCREEN_HEIGHT - 1;
	}
	BACKBUFFER[y * SCREEN_WIDTH + x] = color;
}
