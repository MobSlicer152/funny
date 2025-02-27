#include "base/timer.h"
#include "base/x86.h"
#include "flecs.h"
#include "timer.h"

static void EcsSleep(s32 seconds, s32 nanoseconds)
{
    f32 time = (seconds + nanoseconds / 1000000000.0f) * TIMER_TPS;
    Sleep((u32)time);
}

static void EcsLog(s32 level, cstr file, s32 line, cstr msg)
{
    DebugPrint("[%s] [%s:%d] [%d] %s\n", GetTimeString(), file, line, level, msg);
}

static u64 EcsNow(void)
{
    return GetTimer() * TIMER_SPT * 1000;
}

static void EcsGetTime(ecs_time_t* now)
{
    now->nanosec = GetTimer() * TIMER_SPT * 1e9;
    now->sec = now->nanosec * 1e-9f;
}

static void EcsAbort(void)
{
    HaltAndCatchFire();
}

void InitializeFlecsOsApi(void)
{
    DBG("Initializing flecs OS API");

    ecs_os_api.sleep_ = EcsSleep;

    ecs_os_api.log_ = EcsLog;

    ecs_os_api.abort_ = EcsAbort;
}
