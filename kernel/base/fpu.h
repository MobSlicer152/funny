#pragma once

#define FPU_SINGLE_PRECISION   (0b00 << 8)
#define FPU_DOUBLE_PRECISION   (0b10 << 8)
#define FPU_EXTENDED_PRECISION (0b11 << 8)

#define FPU_EXCEPTION_PRECISION         (1 << 5)
#define FPU_EXCEPTION_UNDERFLOW         (1 << 4)
#define FPU_EXCEPTION_OVERFLOW          (1 << 3)
#define FPU_EXCEPTION_DIVIDE_BY_ZERO    (1 << 2)
#define FPU_EXCEPTION_DENORMAL_OPERAND  (1 << 1)
#define FPU_EXCEPTION_INVALID_OPERATION (1 << 0)

#define FPU_ROUND_NEAREST (0b00 << 10)
#define FPU_ROUND_DOWN    (0b01 << 10)
#define FPU_ROUND_UP      (0b10 << 10)
#define FPU_ROUND_TRUNC   (0b11 << 10)

#define FPU_CONTROL                                                                                                              \
	(FPU_EXTENDED_PRECISION | FPU_EXCEPTION_PRECISION | FPU_EXCEPTION_UNDERFLOW | FPU_EXCEPTION_OVERFLOW |                       \
	 FPU_EXCEPTION_DIVIDE_BY_ZERO | FPU_EXCEPTION_DENORMAL_OPERAND | FPU_EXCEPTION_INVALID_OPERATION | FPU_ROUND_NEAREST)

extern void InitializeFpu(void);
