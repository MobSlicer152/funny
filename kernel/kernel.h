#pragma once

#include <builtins.h>
#include <x86intrin.h>

#include "common/types.h"

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

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)
#define CLAMP(value, min, max) MAX((min), MIN((value), (max)))
