#pragma once

#include "kernel.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

extern void InitializeScreen(void);

extern void Flip(void);
extern void ClearScreen(u8 color);
extern void SetPixel(s32 x, s32 y, u8 color);
