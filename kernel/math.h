#pragma once

#include "kernel.h"

#define PI 3.1415926f
#define TAU (2.0f * PI)

// a bunch of inline functions that do like 1 x87 instruction

static ATTRIBUTE(always_inline) s32 abs(s32 x)
{
    // glibc does this, it's fine somehow (i guess fabs matters more)
    return x < 0 ? -x : x;
}

static ATTRIBUTE(always_inline) f32 fabs(f32 x)
{
    asm volatile ("fabs" : "=t"(x) : "t"(x));
    return x;
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
    asm volatile ("fptan" : "=t"(x) : "t"(x));
    return x;
}
