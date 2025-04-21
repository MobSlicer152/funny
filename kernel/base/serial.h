#pragma once

#include "kernel/macros.h"
#include "kernel/types.h"

#include "cmos.h"
#include "timer.h"

#define SERIAL_115200_BAUD 1
#define SERIAL_57600_BAUD  2
#define SERIAL_38400_BAUD  3

extern void InitializeSerial(u16 baud);
extern void SetBaud(u16 baud);

extern u8 ReadSerial(void);
extern void WriteSerial(const u8* data, usize size);
extern void DebugPrint(cstr msg, ...);
extern void VDebugPrint(cstr msg, va_list args);

#ifdef DEBUG
#define DBG2(file, line, msg, ...)  DebugPrint("[%s] [%d] [" file ":" STRINGIZE(line) "] " msg "\n", GetTimeString(), GetTimer() __VA_OPT__(, ) __VA_ARGS__)
#define DBG(msg, ...) DBG2(__FILE__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__)
#else
#define DBG(...)
#endif
