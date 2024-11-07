#include "libc.h"

// i've written a better memcpy and memset before, but this is easy and fast enough

void* memcpy(void* dest, const void* src, usize size)
{
    asm("rep movsb" : : "D"(dest), "S"(src), "c"(size) : "memory");
    return dest;
}

void* memset(void* dest, s32 value, usize size)
{
    asm("rep stosb" : : "D"(dest), "a"(value), "c"(size));
    return dest;
}
