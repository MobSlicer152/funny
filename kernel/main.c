#include "base/fpu.h"
#include "base/heap.h"
#include "base/idt.h"
#include "base/irq.h"
#include "base/keyboard.h"
#include "base/libc.h"
#include "base/ps2.h"
#include "base/screen.h"
#include "base/serial.h"
#include "base/timer.h"
#include "base/x86.h"

#include "globals/consts.h"
#include "globals/kernel.h"
#include "globals/vars.h"

#include "math/camera.h"
#include "math/math.h"
#include "math/matrix.h"
#include "math/vector.h"

#include "models/cube.h"
#include "models/models.h"

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

	Mat4f_t model;

	Vec4f_t camera = V4F(0.0f, 0.0f, -10.0f, 1.0f);
	Vec4f_t target = V4F(0.0f, 0.0f, 0.0f, 1.0f);
	Mat4f_t view;
	LookAt(view, camera, V4F_UP, target);

	Mat4f_t project;
	Perspective(project, PI / 2.0f, SCREEN_ASPECT, 0.1f, 1000.0f);

	while (true)
	{
		u64 now = GetTimer();

		// only render at framerate, do nothing otherwise
		if ((now - last) > (TIMER_TPS / FPS))
		{
			last = now;

			SwapKeyboardState();

			Mat4RotateY(model, now * TIMER_SPT * TAU);

			ClearScreen(0);
			DrawMesh(&DRAW_INFO(CUBE_VERTICES, CUBE_TEXCOORDS, CUBE_NORMALS, CUBE_FACES, CUBE_FACE_COUNT, model, view, project));
			FlipScreen();
		}
	}

	// shouldn't get here
	HaltAndCatchFire();
}
