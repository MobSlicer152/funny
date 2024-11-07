#pragma once

#include "kernel.h"

#define PI 3.1415926f
#define TAU (2.0f * PI)

// a bunch of inline functions that do like 1 x87 instruction

static ATTRIBUTE(always_inline) s32 abs(s32 x)
{
    return (s32)((u32)x & (-1UL / 2));
}

// https://github.com/embeddedartistry/libc/blob/master/src/math/fabs.c
static ATTRIBUTE(always_inline) f32 fabs(f32 x)
{
    union {f32 f; u32 i;} u = {.i = x};
    u.i &= -1UL / 2; // pretty sure this leaves -0 or smth
    return u.f;
}

static ATTRIBUTE(always_inline) f32 fmod(f32 dividend, f32 divisor)
{
    asm volatile ("fprem1" : "=t"(dividend) : "t"(dividend), "u"(divisor));
    return dividend;
}

static ATTRIBUTE(always_inline) f32 sqrt(f32 x)
{
    asm volatile ("fsqrt" : "=t"(x) : "t"(x));
    return x;
}

static ATTRIBUTE(always_inline) f32 sin(f32 x)
{
    asm volatile ("fsin" : "=t"(x) : "t"(x));
    return x;
}

static ATTRIBUTE(always_inline) f32 cos(f32 x)
{
    asm volatile ("fcos" : "=t"(x) : "t"(x));
    return x;
}

static ATTRIBUTE(always_inline) f32 tan(f32 x)
{
    asm volatile ("ftan" : "=t"(x) : "t"(x));
    return x;
}
