#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <builtins.h>
#include <x86intrin.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;
typedef long double f80;

typedef intptr_t sptr;
typedef uintptr_t uptr;

typedef sptr ssize;
typedef uptr usize;

typedef const char* cstr;
typedef char* dstr;

#define ATTRIBUTE(x) __attribute__((x))

#define KERNEL_BASE 0x7e00
#define STACK_BASE 0x17e00
#define BACKBUFFER_BASE 0x1ae00

#define MAKE_SELECTOR(index, rpl) ((index) << 3 | (rpl))
#define KERNEL_CODE_SELECTOR MAKE_SELECTOR(1, 0)
#define KERNEL_DATA_SELECTOR MAKE_SELECTOR(2, 0)

#define asm __asm__

#define ASSERT(cond)                                                                                                             \
	if (!(cond))                                                                                                                 \
	{                                                                                                                            \
		Halt();                                                                                                                  \
	}
