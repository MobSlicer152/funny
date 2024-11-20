#pragma once

#include "types.h"

extern void InitializeScreen(void);

extern void FlipScreen(void);
extern void ClearScreen(u8 color);
extern void SetPixel(s32 x, s32 y, u8 color);
extern void Fill(s32 x1, s32 y1, s32 x2, s32 y2, u8 color);
extern void DrawBitmap(s32 x, s32 y, const u8* bitmap, s32 width, s32 height);
