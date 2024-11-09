#include "fpu.h"
#include "idt.h"
#include "irq.h"
#include "kernel.h"
#include "libc.h"
#include "math.h"
#include "screen.h"
#include "timer.h"
#include "x86.h"

#define FPS 30

[[noreturn]] void KernelMain(void)
{
	InitializeIdt();
	InitializeIrq();
	EnableInterrupts();
	InitializeFpu();
	InitializeScreen();
	InitializeTimer();

	u64 last = 0;

	while (true)
	{
		u64 now = GetTimer();

		if ((now - last) > (TIMER_TPS / FPS))
		{
			last = now;

			ClearScreen(0);
			for (f32 x = 0; x < SCREEN_WIDTH; x += 0.1)
			{
				f32 cleanX = x / 9.26f + (f32)now / TIMER_TPS;
				f32 cosX = (cos(cleanX) + 1.0f) / 2.0f;
				SetPixel(x, cosX * SCREEN_HEIGHT, 32 + cosX * 15.0f);
				SetPixel(x, -cosX * SCREEN_HEIGHT, 63 - cosX * 15.0f);
			}
			FlipScreen();
		}
	}
}
