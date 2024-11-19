#include "fpu.h"
#include "kernel.h"
#include "serial.h"
#include "x86.h"

#define FPU_SINGLE_PRECISION   (0 << 8)
#define FPU_DOUBLE_PRECISION   (10 << 8)
#define FPU_EXTENDED_PRECISION (11 << 8)

#define FPU_EXCEPTION_PRECISION         (1 << 5)
#define FPU_EXCEPTION_UNDERFLOW         (1 << 4)
#define FPU_EXCEPTION_OVERFLOW          (1 << 3)
#define FPU_EXCEPTION_DIVIDE_BY_ZERO    (1 << 2)
#define FPU_EXCEPTION_DENORMAL_OPERAND  (1 << 1)
#define FPU_EXCEPTION_INVALID_OPERATION (1 << 0)

#define FPU_CONTROL                                                                                                              \
	(FPU_EXTENDED_PRECISION | FPU_EXCEPTION_PRECISION | FPU_EXCEPTION_UNDERFLOW | FPU_EXCEPTION_OVERFLOW |                       \
	 FPU_EXCEPTION_DIVIDE_BY_ZERO | FPU_EXCEPTION_DENORMAL_OPERAND | FPU_EXCEPTION_INVALID_OPERATION)

// This assumes an x87 FPU is present
void InitializeFpu(void)
{
	DebugPrint("Initializing FPU with control word 0x%X\n", FPU_CONTROL);
	// disable x87 emulation and x87 task context saving
	ClearCr0Flag(CR0_EM | CR0_TS);
	SetX87Control(FPU_CONTROL);
}
