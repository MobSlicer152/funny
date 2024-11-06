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

	u32 color = 0;
	for (u32 y = 0; y < 16; y++)
	{
		for (u32 x = 0; x < 16; x++)
		{
			FRAMEBUFFER[(u32)(y / 16.0f * FRAMEBUFFER_HEIGHT) + (u32)(x / 16.0f * FRAMEBUFFER_WIDTH)] = color++;
		}
	}

	while (1) {
		asm ("hlt");
	}
}
