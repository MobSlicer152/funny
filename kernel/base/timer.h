#pragma once

#include "types.h"

#define TIMER_TPS 363
#define TIMER_SPT (1.0f / (f32)TIMER_TPS)

extern void InitializeTimer(void);
extern void UpdateTimer(void);
extern u64 GetTimer(void);
extern void Sleep(u32 ticks);
