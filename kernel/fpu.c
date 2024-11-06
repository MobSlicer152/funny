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

// https://github.com/gcc-mirror/gcc/blob/master/libgcc/floatunsisf.c
f32 __floatunsisf(u32 u)
{
    s32 s = (s32)u;
    if (s < 0)
    {
        return (f32)2.0f * (f32)(s32)((u & 1) | (u >> 1));
    }
    else
    {
        return (f32)s;
    }
}
