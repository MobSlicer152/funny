#include "base/fpu.h"
#include "base/idt.h"
#include "base/irq.h"
#include "base/libc.h"
#include "base/math.h"
#include "base/screen.h"
#include "base/serial.h"
#include "base/timer.h"
#include "base/x86.h"

#include "globals/consts.h"
#include "globals/kernel.h"

#include "sprites/sprites.h"

#define FPS 30

[[noreturn]] void KernelMain(void)
{
	InitializeSerial(SERIAL_115200_BAUD);
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

			ClearScreen(48 + (cos(now * TIMER_SPT) + 1.0f) * 0.5f * 15.0f);
			// for (f32 x = 0; x < SCREEN_WIDTH; x += 0.01f)
			//{
			//	f32 scaledX = x / 9.62f;
			//	f32 y = (cos(scaledX) + 1.0f) * 0.5f;
			//	f32 color = 16 + ((cos(scaledX + (f32)now / TIMER_TPS) + 1.0f) * 0.5f) * 15.0f;
			//	SetPixel(x, y * SCREEN_HEIGHT, color);
			// }
			for (f32 offset = 0.0f; offset < 1.0f; offset += 0.1f)
			{
				f32 angle = offset * TAU + now * TIMER_SPT;
				f32 x = (cos(angle) + 1.0f) * 0.5f * (SCREEN_WIDTH - A_WIDTH);
				f32 y = (sin(angle) + 1.0f) * 0.5f * (SCREEN_HEIGHT - A_HEIGHT);
				DrawBitmap(x, y, A_DATA, A_WIDTH, A_HEIGHT);
			}
			FlipScreen();
		}
	}
}
