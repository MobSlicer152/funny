#pragma once

#include "kernel.h"

#define SERIAL_115200_BAUD 1
#define SERIAL_57600_BAUD 2
#define SERIAL_38400_BAUD 3

extern void InitializeSerial(u16 baud);
extern void SetBaud(u16 baud);

extern u8 ReadSerial(void);
extern void WriteSerial(const u8* data, usize size);
extern void DebugPrint(cstr msg, ...);
extern void VDebugPrint(cstr msg, va_list args);