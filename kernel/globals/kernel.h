#pragma once

#include "types.h"

#define KERNEL_BASE          0x7e00
#define KERNEL_SIZE          0x10000
#define KERNEL_END           (KERNEL_BASE + KERNEL_SIZE)
#define STACK_BASE           KERNEL_END
#define STACK_SIZE           0x4000
#define STACK_END            (STACK_BASE + STACK_SIZE)
#define BACKBUFFER_BASE      STACK_END
#define BACKBUFFER_SIZE      0xfa00
#define BACKBUFFER_END       (BACKBUFFER_BASE + BACKBUFFER_SIZE)
#define INTERRUPT_STACK_BASE BACKBUFFER_END
#define INTERRUPT_STACK_SIZE 0x1000
#define INTERRUPT_STACK_END  (INTERRUPT_STACK_BASE + INTERRUPT_STACK_SIZE)
#define HEAP_BASE            INTERRUPT_STACK_END
#define HEAP_SIZE            0x4000
#define HEAP_END             (HEAP_BASE + HEAP_SIZE)

#define MAKE_SELECTOR(index, rpl) ((index) << 3 | (rpl) & 0b111)
#define KERNEL_CODE_SELECTOR      MAKE_SELECTOR(1, 0)
#define KERNEL_DATA_SELECTOR      MAKE_SELECTOR(2, 0)
