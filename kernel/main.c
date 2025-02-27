#include "base/fpu.h"
#include "base/heap.h"
#include "base/idt.h"
#include "base/irq.h"
#include "base/keyboard.h"
#include "base/libc.h"
#include "base/math.h"
#include "base/ps2.h"
#include "base/screen.h"
#include "base/serial.h"
#include "base/timer.h"
#include "base/x86.h"

#include "globals/consts.h"
#include "globals/kernel.h"
#include "globals/vars.h"

#include "raster/raster.h"

#include "textures/textures.h"

#include "systems/camera.h"
#include "types.h"

#define FPS 30

static void Ring(const u8* bitmap, u32 width, u32 height, u64 now, f32 factor, f32 count)
{
	f32 reciprocal = 1.0f / factor;
	f32 xShift = SCREEN_WIDTH * ((factor - 1) * 0.5f);
	f32 yShift = SCREEN_HEIGHT * ((factor - 1) * 0.5f);
	for (f32 offset = 0.0f; offset < 1.0f; offset += 1.0f / count)
	{
		f32 angle = offset * TAU + now * TIMER_SPT;
		f32 cosAngle = NormalizedCosine(angle);
		f32 sinAngle = NormalizedSine(angle);
		f32 x = (cosAngle * (SCREEN_WIDTH - width) + xShift) * reciprocal;
		f32 y = (sinAngle * (SCREEN_HEIGHT - height) + yShift) * reciprocal;
		Fill(V2I(x, y), V2I(x + width, y + height), 32 + cosAngle * 15.0f);
		DrawBitmap(V2I(x, y), bitmap, width, height);
	}
}

[[noreturn]] void KernelMain(void)
{
	InitializeSerial(SERIAL_115200_BAUD);
	InitializeIdt();
	InitializeIrq();
	EnableInterrupts();
	InitializeFpu();
	InitializeScreen();
	InitializeTimer();
	InitializePs2();
	InitializeKeyboard();

	InitializeHeap(GetKernelHeap(), HEAP_BASE);

	u64 last = 0;

	while (true)
	{
		u64 now = GetTimer();

		if ((now - last) > (TIMER_TPS / FPS))
		{
			last = now;

			SwapKeyboardState();

			ClearScreen(0);

			DrawTriangle(V2I(64, 0), V2I(64, 64), V2I(0, 64), 40);
			DrawTriangle(V2I(0, 0), V2I(64, 0), V2I(0, 64), 32);

			FlipScreen();
		}
	}

	// shouldn't get here
	HaltAndCatchFire();
}
