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
#include "globals/memmap.h"
#include "globals/vars.h"

#include "math/camera.h"
#include "math/math.h"
#include "math/matrix.h"
#include "math/vector.h"

#include "models/models.h"

#include "raster/raster.h"

#include "textures/textures.h"

#include "systems/camera.h"

#include "types.h"

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
	InitializePs2();
	InitializeKeyboard();

	InitializeHeap(GetKernelHeap(), HEAP_BASE);

	u64 last = 0;

	Mat4f_t model = M4F_IDENTITY;
	Mat4f_t translation = M4F_IDENTITY;
	Mat4f_t rotation = M4F_IDENTITY;
	Mat4f_t scale = M4F_IDENTITY;
	Mat4Scale(scale, 0.5f, 0.5f, 0.5f);

	Vec4f_t camera = V4F(0.0f, 5.0f, 10.0f, 1.0f);
	Vec4f_t target = V4F(0.0f, 0.0f, 0.0f, 1.0f);
	Mat4f_t view;
	LookAt(view, camera, V4F_UP, target);

	Mat4f_t project;
	Perspective(project, 78.0f * DEG2RAD, SCREEN_ASPECT, 0.1f, 1000.0f);

	while (true)
	{
		u64 now = GetTimer();

		// only render at framerate, do nothing otherwise
		if ((now - last) > (TIMER_TPS / FPS))
		{
			last = now;

			SwapKeyboardState();

			f32 theta = now * TIMER_SPT * TAU * 0.5f;

			//camera[1] = sin(theta) * 2.5f;
			//LookAt(view, camera, V4F_UP, target);

			Mat4Translate(translation, cos(theta) * 1.5f, 0.0f, 0.0f);
			Mat4RotateY(rotation, theta);
			Mat4Mul(model, translation, rotation);
			Mat4Mul(model, model, scale);

			ClearScreen(32, 1.0f);
			DrawMesh(&DRAW_INFO(
				CUBE_VERTICES, CUBE_TEXCOORDS, CUBE_NORMALS, CUBE_FACES, CUBE_FACE_COUNT, MISSING_DATA, MISSING_WIDTH,
				MISSING_HEIGHT, model, view, project));
			FlipScreen(GetKey(KeyCodeZ));
		}
	}

	// shouldn't get here
	HaltAndCatchFire();
}
