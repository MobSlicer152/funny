#pragma once

#include <float.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#define PRI8(x) "hh" #x
#define PRI16(x) "h" #x
#define PRI32(x) #x
#define PRI64(x) "l" #x

#define PRId8 PRI8("d")
#define PRId16 PRI16("d")
#define PRId32 PRI32("d")
#define PRId64 PRI64("d")

#define PRIu8 PRI8("u")
#define PRIu16 PRI16("u")
#define PRIu32 PRI32("u")
#define PRIu64 PRI64("u")

#define PRIx8 PRI8("x")
#define PRIx16 PRI16("x")
#define PRIx32 PRI32("x")
#define PRIx64 PRI64("x")

typedef float f32;
typedef double f64;
typedef long double f80;

typedef intptr_t sptr;
typedef uintptr_t uptr;

typedef sptr ssize;
typedef uptr usize;

typedef const char* cstr;
typedef char* dstr;