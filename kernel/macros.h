#pragma once

#define MIN(a, b)              (a) < (b) ? (a) : (b)
#define MAX(a, b)              (a) > (b) ? (a) : (b)
#define CLAMP(value, min, max) MAX((min), MIN((value), (max)))
#define SWAP(a, b)                                                                                                               \
	{                                                                                                                            \
		typeof(a) c = (b);                                                                                                       \
		(b) = (a);                                                                                                               \
		(a) = c;                                                                                                                 \
	}

#define STRINGIZE(x)         #x
#define STRINGIZE_EXPAND(x)  STRINGIZE(x)
#define STRINGIZE_EXPAND2(x) STRINGIZE_EXPAND(x)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define ATTRIBUTE(x) __attribute__((x))

#define asm __asm__

#ifndef KERNEL
#define Halt abort
#endif

#define ASSERT(cond)                                                                                                             \
	if (!(cond))                                                                                                                 \
	{                                                                                                                            \
		DBG("Assertion \"" #cond "\" failed!");                                                                                  \
		DisableInterrupts();                                                                                                     \
		Halt();                                                                                                                  \
	}
