#include "libc.h"
#include "math.h"
#include "screen.h"

static volatile u8* const FRAMEBUFFER = (volatile u8* const)0xA0000;
static u8 s_backbuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

void InitializeScreen(void)
{
    ClearScreen(0);
    Flip();
}

void Flip(void)
{
    memcpy((u8*)FRAMEBUFFER, s_backbuffer, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void ClearScreen(u8 color)
{
    // memset is the fastest for 
    memset(s_backbuffer, color, SCREEN_WIDTH * SCREEN_HEIGHT);
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
    s_backbuffer[y * SCREEN_WIDTH + x] = color;
}
