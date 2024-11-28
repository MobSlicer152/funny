#pragma once

#include "types.h"

extern void InitializePs2(void);
extern bool Ps2Initialized(void);

extern u8 GetPs2Status(void);
extern u8 ReadPs2Data(void);
extern void WritePs2Data(u8 data);
