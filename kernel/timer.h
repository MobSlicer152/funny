#pragma once

#include "kernel.h"

#define TIMER_TPS 363

extern void InitializeTimer(void);
extern void UpdateTimer(void);
extern u64 GetTimer(void);
extern void Sleep(u32 ticks);
