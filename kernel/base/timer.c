// based on https://github.com/lucianoforks/tetris-os/blob/master/src/timer.c

#include "serial.h"
#include "timer.h"
#include "x86.h"

#define PIT_BASE 0x40
#define PIT_CHANNEL0 (PIT_BASE + 0x0)
#define PIT_CHANNEL1 (PIT_BASE + 0x1)
#define PIT_CHANNEL2 (PIT_BASE + 0x2)
#define PIT_CONTROL (PIT_BASE + 0x3)

#define PIT_SET 0x36

#define PIT_FREQUENCY 1193131.666

static u64 s_ticks;

static void SetFrequency(u32 hertz)
{
    DBG("Setting timer frequency to %u Hz", hertz);
    OutByte(PIT_CONTROL, PIT_SET);
    u16 divisor = (u16)(PIT_FREQUENCY / hertz);
    OutByte(PIT_CHANNEL0, divisor & 0xFF);
    OutByte(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}

void InitializeTimer(void)
{
    s_ticks = 0;

    SetFrequency(TIMER_TPS);
}

void UpdateTimer(void)
{
    s_ticks++;
}

u64 GetTimer(void)
{
    return s_ticks;
}
