#pragma once

#include "globals/vars.h"
#include "types.h"

typedef struct Camera
{
	f32 x;
	f32 y;
} Camera_t;

#define PROJECT_ENTITY(entity, outX, outY, outWidth, outHeight)                                                                  \
	Project((entity)[0], (entity)[1], (entity)->width, (entity)->height, &(outX), &(outY), &(outWidth), &(outHeight))
extern void Project(f32 x, f32 y, f32 width, f32 height, u32* outX, u32* outY, u32* outWidth, u32* outHeight);
extern bool Visible(f32 x, f32 y, f32 width, f32 height);
