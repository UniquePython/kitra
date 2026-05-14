#include "kitra_internal.h"

KitraTimer KitraStartTimer(void)
{
    uint64_t now = SDL_GetPerformanceCounter();

    return (KitraTimer){
        .start = now,
        .last = now,
        .repeat = false};
}

void KitraResetTimer(KitraTimer *t)
{
    if (!t)
        return;

    uint64_t now = SDL_GetPerformanceCounter();

    t->start = now;
    t->last = now;
}

float KitraGetTimerElapsed(const KitraTimer *t)
{
    if (!t)
        return 0.0f;

    uint64_t now = SDL_GetPerformanceCounter();

    return (float)((double)(now - t->start) / (double)gKitraCtx.timing.perfFrequency);
}

bool KitraTimerDone(KitraTimer *t, float duration)
{
    if (!t)
        return false;

    uint64_t now = SDL_GetPerformanceCounter();

    float elapsed = (float)((double)(now - t->last) / (double)gKitraCtx.timing.perfFrequency);

    if (elapsed >= duration)
    {
        if (t->repeat)
            t->last += (uint64_t)((double)duration * (double)gKitraCtx.timing.perfFrequency);

        return true;
    }

    return false;
}

void KitraTimerSetRepeat(KitraTimer *t, bool repeat)
{
    if (!t)
        return;

    t->repeat = repeat;
}

float KitraGetDeltaTime(void)
{
    return gKitraCtx.timing.deltaTime;
}

void KitraSetTargetFPS(int fps)
{
    gKitraCtx.timing.targetFPS = fps;
}

float KitraGetFPS(void)
{
    return gKitraCtx.timing.fps;
}