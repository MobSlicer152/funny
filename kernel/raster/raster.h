#pragma once

#include "kernel/types.h"
#include "kernel/math/vector.h"

extern void DrawTriangle(const Vec2i_t v0, const Vec2i_t v1, const Vec2i_t v2, u8 color);
extern void DrawMesh(const Vec3f_t* vertices, const Vec3i_t* indices, const u8* colors, usize triangleCount);
