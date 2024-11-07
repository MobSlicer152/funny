#include "fpu.h"
#include "idt.h"
#include "irq.h"
#include "kernel.h"
#include "timer.h"

_Noreturn void KernelMain(void)
{
	static volatile u8* const FRAMEBUFFER = (volatile u8* const)0xA0000;
	static const u32 FRAMEBUFFER_WIDTH = 320;
	static const u32 FRAMEBUFFER_HEIGHT = 200;

	InitializeIdt();
	InitializeIrq();
	InitializeFpu();
	//InitializeTimer();

	for (u32 y = 0; y < FRAMEBUFFER_HEIGHT; y++)
	{
		for (u32 x = 0; x < FRAMEBUFFER_WIDTH; x++)
		{
			FRAMEBUFFER[y * FRAMEBUFFER_WIDTH + x] = (u8)(((f32)x / FRAMEBUFFER_WIDTH + (f32)y / FRAMEBUFFER_HEIGHT) * 15.0f) + 32;
		}
	}

	while (1) {
		asm ("hlt");
	}
}
