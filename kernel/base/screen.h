#pragma once

#include "kernel/types.h"
#include "vector.h"

extern void InitializeScreen(void);

extern void FlipScreen(void);
extern void ClearScreen(u8 color);
extern void FixPoint(Vec2i_t p);
extern void SetPixel(const Vec2i_t p, u8 color);
extern void RawSetPixel(const Vec2i_t p, u8 color);
extern void Fill(const Vec2i_t p1, const Vec2i_t p2, u8 color);
extern void DrawBitmap(const Vec2i_t p, const u8* bitmap, s32 width, s32 height);
