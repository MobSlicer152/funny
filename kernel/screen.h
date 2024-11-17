#pragma once

#include "kernel.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

extern void InitializeScreen(void);

extern void FlipScreen(void);
extern void ClearScreen(u8 color);
extern void SetPixel(s32 x, s32 y, u8 color);
extern void Fill(s32 x1, s32 y1, s32 x2, s32 y2, u8 color);
