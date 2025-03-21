#pragma once

#define MIN(a, b)              (a) < (b) ? (a) : (b)
#define MAX(a, b)              (a) > (b) ? (a) : (b)
#define MIN3(a, b, c)          MIN(a, MIN(b, c))
#define MAX3(a, b, c)          MAX(a, MAX(b, c))
#define CLAMP(value, min, max) MAX((min), MIN((value), (max)))
#define SWAP(a, b)                                                                                                               \
	{                                                                                                                            \
		typeof(a) c = (b);                                                                                                       \
		(b) = (a);                                                                                                               \
		(a) = c;                                                                                                                 \
	}

#define MAKE_SELECTOR(index, rpl) ((index) << 3 | (rpl) & 0b111)
#define KERNEL_CODE_SELECTOR      MAKE_SELECTOR(1, 0)
#define KERNEL_DATA_SELECTOR      MAKE_SELECTOR(2, 0)

#define STRINGIZE(x)         #x
#define STRINGIZE_EXPAND(x)  STRINGIZE(x)
#define STRINGIZE_EXPAND2(x) STRINGIZE_EXPAND(x)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define ATTRIBUTE(x) __attribute__((x))
#define FORCEINLINE  ATTRIBUTE(always_inline)

#define asm __asm__

#ifndef KERNEL
#define HaltAndCatchFire abort
#endif

#define UNUSED(x) ((void)x)

#define ASSERT(cond, ...)                                                                                                        \
	if (!(cond))                                                                                                                 \
	{                                                                                                                            \
		DBG("Assertion \"" #cond "\" failed: " #__VA_ARGS__);                                                                    \
		DisableInterrupts();                                                                                                     \
		HaltAndCatchFire();                                                                                                      \
	}
