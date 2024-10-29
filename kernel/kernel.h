#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef X86
#include <x86intrin.h>
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef intptr_t sptr;
typedef uintptr_t uptr;

typedef sptr ssize;
typedef uptr usize;

typedef const char* cstr;
typedef char* dstr;
