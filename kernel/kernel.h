#pragma once

#include <builtins.h>
#include <x86intrin.h>

#include "common/macros.h"
#include "common/types.h"

#define ATTRIBUTE(x) __attribute__((x))

#define KERNEL_BASE          0x7e00
#define STACK_BASE           0x17e00
#define STACK_SIZE           0x4000
#define STACK_END            (STACK_BASE + STACK_SIZE)
#define BACKBUFFER_BASE      0x1be00
#define BACKBUFFER_SIZE      0xfa00
#define INTERRUPT_STACK_BASE 0x2b800
#define INTERRUPT_STACK_SIZE 0x1000
#define INTERRUPT_STACK_END  (INTERRUPT_STACK_BASE + INTERRUPT_STACK_SIZE)

#define MAKE_SELECTOR(index, rpl) ((index) << 3 | (rpl))
#define KERNEL_CODE_SELECTOR      MAKE_SELECTOR(1, 0)
#define KERNEL_DATA_SELECTOR      MAKE_SELECTOR(2, 0)

#define asm __asm__

#define ASSERT(cond)                                                                                                             \
	if (!(cond))                                                                                                                 \
	{                                                                                                                            \
		Halt();                                                                                                                  \
	}
