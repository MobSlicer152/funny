#include "fpu.h"
#include "idt.h"
#include "kernel.h"

_Noreturn void KernelMain(void)
{
	static volatile u8* const FRAMEBUFFER = (volatile u8* const)0xA0000;
	static const u32 FRAMEBUFFER_WIDTH = 320;
	static const u32 FRAMEBUFFER_HEIGHT = 200;

	InitializeIdt();
	InitializeFpu();

	u16 color = 0;
	for (u32 y = 0; y < FRAMEBUFFER_HEIGHT; y++)
	{
		for (u32 x = 0; x < FRAMEBUFFER_WIDTH; x++)
		{
			FRAMEBUFFER[y * FRAMEBUFFER_WIDTH + x] = color++ / 256;
		}
	}

	while (1) {
		__asm__ ("hlt");
	}
}
