#pragma once

#include "types.h"

// 480.5 MiB available from 0x7e00 to 0x7ffff

// kernel binary
#define KERNEL_BASE          0x7e00
#define KERNEL_SIZE          0x10000
#define KERNEL_END           (KERNEL_BASE + KERNEL_SIZE)

// general stack (update startup.asm if changed)
#define STACK_BASE           KERNEL_END
#define STACK_SIZE           0x4000
#define STACK_END            (STACK_BASE + STACK_SIZE)

// vga backbuffer
#define BACKBUFFER_BASE      STACK_END
#define BACKBUFFER_SIZE      0xfa00
#define BACKBUFFER_END       (BACKBUFFER_BASE + BACKBUFFER_SIZE)

// z buffer
#define ZBUFFER_BASE         BACKBUFFER_END
// fa00 * sizeof(Depth_t)
#define ZBUFFER_SIZE         0x1f400
#define ZBUFFER_END          (ZBUFFER_BASE + ZBUFFER_SIZE)

// isr stack
#define INTERRUPT_STACK_BASE ZBUFFER_END
#define INTERRUPT_STACK_SIZE 0x1000
#define INTERRUPT_STACK_END  (INTERRUPT_STACK_BASE + INTERRUPT_STACK_SIZE)

// heap
#define HEAP_BASE            INTERRUPT_STACK_END
#define HEAP_SIZE            0x4000
#define HEAP_END             (HEAP_BASE + HEAP_SIZE)
