#include "camera.h"
#include "globals/consts.h"
#include "globals/vars.h"

void Project(f32 x, f32 y, f32 width, f32 height, u32* outX, u32* outY, u32* outWidth, u32* outHeight)
{
	f32 centerX = 0.0f;
	f32 centerY = 0.0f;

	*outWidth = width * SPRITE_SIZE;
	*outHeight = height * SPRITE_SIZE;
	centerX = *outWidth * 0.5f;
	centerY = *outHeight * 0.5f;

	f32 cameraX = (x - g_camera.x) * SPRITE_SIZE - centerX;
	f32 cameraY = (y - g_camera.y) * SPRITE_SIZE + centerY;

	*outX = cameraX + SCREEN_WIDTH * 0.5f;
	*outY = -cameraY + SCREEN_HEIGHT * 0.5f;
}

bool Visible(f32 x, f32 y, f32 width, f32 height)
{
	u32 projX = 0.0f;
	u32 projY = 0.0f;
	u32 projWidth = 0.0f;
	u32 projHeight = 0.0f;
	Project(x, y, width, height, &projX, &projY, &projWidth, &projHeight);
	return (projX + projWidth) * 0.5f > -(SCREEN_WIDTH * 0.5f) && (projX - projWidth) * 0.5f < (SCREEN_WIDTH * 0.5f) &&
		   (projY + projHeight) * 0.5f > -(SCREEN_HEIGHT * 0.5f) && (projY - projHeight) * 0.5f < (SCREEN_HEIGHT * 0.5f);
}
