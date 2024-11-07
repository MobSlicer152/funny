#pragma once

#include "kernel.h"

#define MAKE_IN_FUNC(name, size, suffix)                                                                                         \
	static ATTRIBUTE(always_inline) u##size name(u16 port)                                                                              \
	{                                                                                                                            \
		u##size value = 0;                                                                                                       \
		asm volatile("in" #suffix " %1, %0" : "=a"(value) : "d"(port) : "memory");                                               \
		return value;                                                                                                            \
	}

MAKE_IN_FUNC(InByte, 8, b)
MAKE_IN_FUNC(InWord, 16, w)
MAKE_IN_FUNC(InDword, 32, l)

#undef MAKE_IN_FUNC

#define MAKE_OUT_FUNC(name, size, suffix) static ATTRIBUTE(always_inline) void name(u16 port, u##size value) { asm volatile("out" #suffix " %0, %1" : : "a"(value), "d"(port)); }

MAKE_OUT_FUNC(OutByte, 8, b)
MAKE_OUT_FUNC(OutWord, 16, w)
MAKE_OUT_FUNC(OutDword, 32, l)

#undef MAKE_OUT_FUNC

#define CR0_PE (1 << 0)  // protected mode enable
#define CR0_MP (1 << 1)  // monitor coprocessor
#define CR0_EM (1 << 2)  // x87 emulation
#define CR0_TS (1 << 3)  // task switched
#define CR0_ET (1 << 4)  // extension type
#define CR0_NE (1 << 5)  // numeric error
#define CR0_WP (1 << 16) // write protect
#define CR0_AM (1 << 18) // alignment mask
#define CR0_NW (1 << 29) // not write through
#define CR0_CD (1 << 30) // cache disable
#define CR0_PG (1 << 31) // paging

static ATTRIBUTE(always_inline) u32 ReadCr0(void)
{
	u32 value = 0;
	asm volatile("mov %%cr0, %0" : "=r"(value));
	return value;
}

static ATTRIBUTE(always_inline) void WriteCr0(u32 value)
{
	asm volatile("mov %0, %%cr0" : "=r"(value));
}

static ATTRIBUTE(always_inline) void SetCr0Flag(u32 flag)
{
	WriteCr0(ReadCr0() | flag);
}

static ATTRIBUTE(always_inline) void ClearCr0Flag(u32 flag)
{
	WriteCr0(ReadCr0() & ~flag);
}

static ATTRIBUTE(always_inline) void IoWait(void)
{
	// write to dummy port
	OutByte(0x80, 0);
}

static ATTRIBUTE(always_inline) u16 GetX87Status(void)
{
	u16 value = 0;
	asm volatile("fnstsw %0" : "=r"(value));
	return value;
}

static ATTRIBUTE(always_inline) void SetX87Control(u16 value)
{
	asm volatile("fldcw %0" : : "m"(value));
}

static ATTRIBUTE(always_inline) void ResetX87Control(void)
{
	asm volatile("fninit");
}