#pragma once

#include "kernel/macros.h"
#include "math.h"
#include "matrix.h"
#include "vector.h"

// https://github.com/recp/cglm/blob/master/include/cglm/clipspace/view_lh.h glm_lookat_lh
static FORCEINLINE void LookAt(Mat4f_t result, const Vec4f_t camera, const Vec4f_t up, const Vec4f_t target)
{
	Vec4f_t f;
	Vec4fSub(f, target, camera);
	Vec4fNormalize(f, f);

	Vec4f_t s;
	Vec4fCross(s, up, f);
	Vec4fNormalize(s, s);

	Vec4f_t u;
	Vec4fCross(u, f, s);
	Vec4fNormalize(u, u);

	// sc = dot(s, camera)
	// uc = dot(u, camera)
	// fc = dot(f, camera)
	//  s0  u0  f0 0
	//  s1  u1  f1 0
	//  s2  u2  f2 0
	// -sc -uc -fc 1
	M4F_COPY(
		result, M4F(s[0], u[0], f[0], 0.0f, s[1], u[1], f[1], 0.0f, s[2], u[2], f[2], 0.0f, -Vec4fDot(s, camera),
					-Vec4fDot(u, camera), -Vec4fDot(f, camera), 1.0f));
}

static FORCEINLINE void Look(Mat4f_t result, const Vec4f_t camera, const Vec4f_t up, const Vec4f_t direction)
{
	Vec4f_t target;
	Vec4fAdd(target, camera, direction);
	LookAt(result, camera, up, target);
}

static FORCEINLINE void Perspective(Mat4f_t result, f32 fov, f32 aspect, f32 nearZ, f32 farZ)
{
	f32 scale = 1.0f / tan(fov * 0.5f); // Scale factor based on FOV
	f32 range = 1.0f / (farZ - nearZ);  // Depth range

	M4F_COPY(result, M4F(scale / aspect, 0, 0, 0, 0, scale, 0, 0, 0, 0, farZ * range, 1.0f, 0, 0, -nearZ * farZ * range, 0));
}
