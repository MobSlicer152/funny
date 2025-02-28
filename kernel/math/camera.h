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

    // sc = dot(s, camera)
    // uc = dot(u, camera)
    // fc = dot(f, camera)
    //  s0  s1  s2 0
    //  u0  u1  u2 0
    //  f0  f1  f2 0
    // -sc -uc -fc 0
    M4F_COPY(result, M4F(s[0], s[1], s[2], 0.0f, u[0], u[1], u[2], 0.0f, f[0], f[1], f[2], 0.0f, -Vec4fDot(s, camera), -Vec4fDot(u, camera), -Vec4fDot(f, camera), 1.0f));
}

static FORCEINLINE void Look(Mat4f_t result, const Vec4f_t camera, const Vec4f_t up, const Vec4f_t direction)
{
    Vec4f_t target;
    Vec4fAdd(target, camera, direction);
    LookAt(result, camera, up, target);
}

static FORCEINLINE void Perspective(Mat4f_t result, f32 fov, f32 aspect, f32 nearZ, f32 farZ)
{
    f32 scale = 1.0f / tan(fov * 0.5f);
    f32 farNear = 1.0f / (farZ - nearZ);

    M4F_COPY(result, M4F(scale / aspect, 0, 0, 0, 0, scale, 0, 0, 0, 0, -(nearZ + farZ) * farNear, -1, 0, 0, 2 * farZ * nearZ * farNear, 0.0f));
}
