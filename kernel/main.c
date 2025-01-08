#include "base/fpu.h"
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

#include "sprites/a.h"
#include "sprites/sprites.h"

#include "systems/camera.h"

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
		Fill(x, y, x + width, y + height, 32 + cosAngle * 15.0f);
		DrawBitmap(x, y, bitmap, width, height);
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

	u64 last = 0;
	f32 scale = 1.0f;

	while (true)
	{
		u64 now = GetTimer();

		if ((now - last) > (TIMER_TPS / FPS))
		{
			last = now;

			// so it doesn't change mid frame
			SwapKeyboardState();

			ClearScreen(16 + (cos(now * TIMER_SPT) + 1.0f) * 0.5f * 15.0f);

			if (GetKey(KeyCodeA))
			{
				if (scale > 1.05f)
				{
					scale -= 0.05f;
				}
			}
			if (GetKey(KeyCodeD))
			{
				if (scale < 9.95f)
				{
					scale += 0.05f;
				}
			}

			Ring(A_DATA, A_WIDTH, A_HEIGHT, now, scale, 80.0f / scale);
			FlipScreen();

			u32 light = 1 << (u32)(NormalizedCosine(now * TIMER_SPT) * 3.0f);
			SetKeyboardLights(light & 0b001, light & 0b010, light & 0b100);
		}
	}

	// shouldn't get here
	HaltAndCatchFire();
}
