#pragma once

#include "kernel.h"

#define PI 3.1415926f
#define TAU (2.0f * PI)

// a bunch of inline functions that do like 1 x87 instruction

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
