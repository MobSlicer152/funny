#include "fpu.h"
#include "kernel.h"

f32 sinf(f32 x)
{
    asm volatile ("fsin" : : "f"(x));
    return x;
}
