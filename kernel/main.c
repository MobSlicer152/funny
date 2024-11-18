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
			Fill(10, 10, 30, 30, 15);
			for (f32 x = 0; x < SCREEN_WIDTH; x += 0.01f)
			{
				f32 scaledX = x / 9.62f;
				f32 y = (cos(scaledX) + 1.0f) / 2.0f;
				f32 color = 16 + ((cos(scaledX + (f32)now / TIMER_TPS) + 1.0f) / 2.0f) * 15.0f;
				SetPixel(x, y * SCREEN_HEIGHT, color);
			}
			FlipScreen();
		}
	}
}
