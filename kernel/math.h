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

// https://stackoverflow.com/a/57228953
static ATTRIBUTE(always_inline) f32 fpow(f32 x, f32 y) {
    f32 result;
    asm volatile (
        // Save the address of 'y' in %eax
        "lea 12(%%esp), %%eax;" // Load 'y' (exponent) into the FPU stack
        "fldl (%%eax);" // Round 'y' back to an integer for odd/even bit testing
        "fistl (%%eax);" // Get 'x' sign bit and 'y' odd/even bit
        "movzwl -2(%%eax), %%eax;" // Load 'x' (base) into the FPU stack
        "fldl 4(%%esp);" // Take the absolute value of 'x' (make positive)
        "fabs;" // Mask off bits except for 'y' odd and 'x' sign
        "and $0x0180, %%ax;" // Compute 'y * log2(x)' (pops ST(1))
        "fyl2x;" // Load 1.0 for later use
        "fld1;" // Duplicate the current FPU stack value
        "fld %%st(1);" // Extract the mantissa
        "fprem1;" // Compute (2^ST(0) - 1)
        "f2xm1;" // Add 1.0 to compute (2^ST(0))
        "faddp %%st(1), %%st;" // Scale the result by 2^(integer part of ST(1))
        "fscale;" // Free the FPU stack to balance
        "ffree %%st(1);" // Test if 'y' is odd and 'x' is negative
        "cmp $0x0180, %%ax;" // If not, jump to the end
        "jne 1f;" // Otherwise, negate the result
        "fchs;" // Label 1
        "1:;"
        : "=t"(result) // Output result in FPU stack top (ST(0))
        : "m"(x), "m"(y) // Input variables
        : "eax", "ax", "memory" // Clobbered registers and memory
    );
    return result;
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
