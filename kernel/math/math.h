#pragma once

#include "base/fpu.h"
#include "base/x86.h"

#include "kernel/macros.h"
#include "kernel/types.h"

#define INFINITY (1.0f / 0.0f)
#define NAN      (0.0f / 0.0f)

#define PI      3.14159265359f
#define HALF_PI 1.57079632679f
#define TAU     6.28318530718f

#define DEG2RAD (TAU / 360.0f)
#define RAD2DEG (360.0f / TAU)

// a bunch of inline functions that do like 1 x87 instruction

static FORCEINLINE f32 round(f32 x)
{
	asm volatile("frndint" : "=t"(x) : "t"(x));
	return x;
}

static FORCEINLINE f32 floor(f32 x)
{
	SetRoundingMode(FPU_ROUND_DOWN);
	f32 result = round(x);
	SetRoundingMode(FPU_ROUND_NEAREST);
	return result;
}

static FORCEINLINE f32 ceil(f32 x)
{
	SetRoundingMode(FPU_ROUND_UP);
	f32 result = round(x);
	SetRoundingMode(FPU_ROUND_NEAREST);
	return result;
}

static FORCEINLINE s32 abs(s32 x)
{
	// glibc does this, it's fine somehow (i guess fabs matters more)
	return x < 0 ? -x : x;
}

static FORCEINLINE f32 fabs(f32 x)
{
	asm volatile("fabs" : "=t"(x) : "t"(x));
	return x;
}

static FORCEINLINE f32 fmod(f32 dividend, f32 divisor)
{
	asm volatile("fprem1" : "=t"(dividend) : "t"(dividend), "u"(divisor));
	return dividend;
}

// https://stackoverflow.com/a/57228953
static FORCEINLINE f32 fpow(f32 x, f32 y)
{
	f32 result;
	asm volatile(
		// Save the address of 'y' in %eax
		"lea 12(%%esp), %%eax;"    // Load 'y' (exponent) into the FPU stack
		"fldl (%%eax);"            // Round 'y' back to an integer for odd/even bit testing
		"fistl (%%eax);"           // Get 'x' sign bit and 'y' odd/even bit
		"movzwl -2(%%eax), %%eax;" // Load 'x' (base) into the FPU stack
		"fldl 4(%%esp);"           // Take the absolute value of 'x' (make positive)
		"fabs;"                    // Mask off bits except for 'y' odd and 'x' sign
		"and $0x0180, %%ax;"       // Compute 'y * log2(x)' (pops ST(1))
		"fyl2x;"                   // Load 1.0 for later use
		"fld1;"                    // Duplicate the current FPU stack value
		"fld %%st(1);"             // Extract the mantissa
		"fprem1;"                  // Compute (2^ST(0) - 1)
		"f2xm1;"                   // Add 1.0 to compute (2^ST(0))
		"faddp %%st(1), %%st;"     // Scale the result by 2^(integer part of ST(1))
		"fscale;"                  // Free the FPU stack to balance
		"ffree %%st(1);"           // Test if 'y' is odd and 'x' is negative
		"cmp $0x0180, %%ax;"       // If not, jump to the end
		"jne 1f;"                  // Otherwise, negate the result
		"fchs;"                    // Label 1
		"1:;"
		: "=t"(result)          // Output result in FPU stack top (ST(0))
		: "m"(x), "m"(y)        // Input variables
		: "eax", "ax", "memory" // Clobbered registers and memory
	);
	return result;
}

static FORCEINLINE f32 invsqrt(f32 x)
{
	// we love quake 3
	f32 x2 = x * 0.5;
	f32 y = x;
	u32 i = *(u32*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(f32*)&i;
	y = y * (1.5 - (x2 * y * y));
	return y;
}

static FORCEINLINE f32 sqrt(f32 x)
{
	// as it turns out, fast inverse square root is faster than fsqrt by a lot
	// (which is to be expected, given the name and it being made when x87 was prevalent)
	// asm volatile ("fsqrt" : "=t"(x) : "t"(x));
	return 1.0f / invsqrt(x);
}

static FORCEINLINE f32 sin(f32 x)
{
	asm volatile("fsin" : "=t"(x) : "t"(x));
	return x;
}

static FORCEINLINE f32 NormalizedSine(f32 x)
{
	return (sin(x) + 1.0f) * 0.5f;
}

static FORCEINLINE f32 cos(f32 x)
{
	asm volatile("fcos" : "=t"(x) : "t"(x));
	return x;
}

static FORCEINLINE f32 NormalizedCosine(f32 x)
{
	return (cos(x) + 1.0f) * 0.5f;
}

static FORCEINLINE f32 tan(f32 x)
{
	return sin(x) / cos(x);
}

static FORCEINLINE bool isinf(f32 x)
{
	u32 xi = *(u32*)&x;
	// zero fraction, max exponent
	return (xi & 0x7f800000) == 0x7f800000 && (xi & 0x007fffff) == 0;
}

static FORCEINLINE bool isnan(f32 x)
{
	u32 xi = *(u32*)&x;
	// non-zero fraction, max exponent
	return (xi & 0x7f800000) == 0x7f800000 && (xi & 0x007fffff) != 0;
}
