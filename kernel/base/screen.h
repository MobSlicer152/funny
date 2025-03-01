#pragma once

#include "kernel/types.h"
#include "kernel/math/vector.h"

extern void InitializeScreen(void);

extern void FlipScreen(bool showZBuffer);
extern void ClearScreen(u8 color, u8 depth);
extern void FixPoint(Vec2i_t p);
extern void SetPixel(const Vec2i_t p, u8 color);
extern void RawSetPixel(const Vec2i_t p, u8 color);
extern void WriteZBuffer(const Vec2i_t p, u8 value);
extern u8 ReadZBuffer(const Vec2i_t p);
extern void Fill(const Vec2i_t p1, const Vec2i_t p2, u8 color);
extern void DrawBitmap(const Vec2i_t p, const u8* bitmap, s32 width, s32 height);
