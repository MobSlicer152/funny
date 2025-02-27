#pragma once

#include "kernel/macros.h"
#include "kernel/types.h"
#define STB_SPRINTF_DECORATE(x) x
#include "kernel/stb_sprintf.h"

extern void* memcpy(void* dest, const void* source, usize size);
extern void* memmove(void* dest, const void* source, usize size);
extern void* memset(void* dest, s32 value, usize size);
extern int memcmp(const void* a, const void* b, usize count);

extern usize strlen(cstr str);

extern s32 strncmp(cstr a, cstr b, usize count);
extern s32 strcmp(cstr a, cstr b);

extern cstr strchr(cstr str, char c);
extern cstr strrchr(cstr str, char c);
extern cstr strstr(cstr str, cstr seq);

#define strcat(a, b) ASSERT(false, "don't use strcat")
#define strcpy(a, b) ASSERT(false, "don't use strcpy")

extern dstr strncpy(dstr dest, cstr src, usize size);

extern s64 atoll(cstr str);
extern s32 atoi(cstr str);
extern f32 atof(cstr str);

#define atol atoi
#define strtol(str, end, base) atoi(str)

#define RAND_MAX INT32_MAX

extern void srand(s32 seed);
extern s32 rand(void);

static FORCEINLINE bool isupper(char c)
{
    return 'A' <= c && c <= 'Z';
}

static FORCEINLINE char tolower(char c)
{
    return isupper(c) ? c ^ 0x20 : c;
}

static FORCEINLINE bool islower(char c)
{
    return 'a' <= c && c <= 'z';
}

static FORCEINLINE char toupper(char c)
{
    return islower(c) ? c ^ 0x20 : c;
}

static FORCEINLINE bool isalpha(char c)
{
    return isupper(c) || islower(c);
}

static FORCEINLINE bool isspace(char c)
{
    return '\0' < c && c <= ' ';
}

static FORCEINLINE bool isdigit(char c)
{
    return '0' <= c && c <= '9';
}

static FORCEINLINE bool isxdigit(char c)
{
    return isdigit(c) || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f');
}

#define printf DBG

#define alloca(size) __builtin_alloca(size)
