#include "fpu.h"
#include "idt.h"
#include "irq.h"
#include "kernel.h"
#include "libc.h"
#include "math.h"
#include "screen.h"
#include "timer.h"

_Noreturn void KernelMain(void)
{
	InitializeIdt();
	//InitializeIrq();
	InitializeFpu();
	InitializeScreen();
	//InitializeTimer();

	for (u32 x = 0; x < SCREEN_WIDTH; x++)
	{
		f32 cleanX = fmod((f32)x / 9.26f, 63.0);
		f32 sinX = (sin(cleanX) + 1.0f) / 2.0f;
		//f32 cosX = (cos(cleanX) + 1.0f) / 2.0f;
		SetPixel(x, sinX * SCREEN_HEIGHT, 32 + sinX * 15.0f);
		SetPixel(x, SCREEN_HEIGHT - (sinX * SCREEN_HEIGHT), 63 - sinX * 15.0f);
	}
	Flip();

	while (1) {
		asm ("hlt");
	}
}
