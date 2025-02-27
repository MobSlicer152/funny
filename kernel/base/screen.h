#pragma once

#include "types.h"

extern void InitializeScreen(void);

extern void FlipScreen(void);
extern void ClearScreen(u8 color);
extern void FixPoint(Point2i_t* p);
extern void SetPixel(const Point2i_t* p, u8 color);
extern void RawSetPixel(const Point2i_t* p, u8 color);
extern void Fill(const Point2i_t* p1, const Point2i_t* p2, u8 color);
extern void DrawBitmap(const Point2i_t* p, const u8* bitmap, s32 width, s32 height);
