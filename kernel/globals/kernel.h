#pragma once

#define KERNEL_BASE          0x7e00
#define KERNEL_SIZE          0x10000
#define KERNEL_END           (KERNEL_BASE + KERNEL_SIZE)
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
