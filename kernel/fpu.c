#include "fpu.h"
#include "kernel.h"
#include "x86.h"

// This assumes an x87 FPU is present
void InitializeFpu(void)
{
    // disable x87 emulation and task switched
    ClearCr0Flag(CR0_EM | CR0_TS);
    SetX87Control(0x37a); // according to osdev wiki, this enables divide by zero and invalid operation exceptions
}
