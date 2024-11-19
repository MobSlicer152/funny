#pragma once

#include "common/types.h"
#define STB_SPRINTF_DECORATE(x) x
#include "common/stb_sprintf.h"


extern void* memcpy(void* dest, const void* source, usize size);
extern void* memset(void* dest, s32 value, usize size);
