#pragma once

#include "kernel/types.h"

extern u8 ReadCmosRegister(u8 index);
extern void ReadRtc(u32* seconds, u32* minutes, u32* hours, u32* day, u32* month, u32* year);
extern cstr GetTimeString(void);
