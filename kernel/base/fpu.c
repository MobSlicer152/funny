#include "fpu.h"
#include "serial.h"
#include "x86.h"

// This assumes an x87 FPU is present
void InitializeFpu(void)
{
	DBG("Initializing FPU with control word 0x%X", FPU_CONTROL);
	// disable x87 emulation and x87 task context saving
	ClearCr0Flag(CR0_EM | CR0_TS);
	SetX87Control(FPU_CONTROL);
}
