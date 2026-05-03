#include "cinder_internal.h"

CinderTimer CinderStartTimer(void)
{
    uint64_t now = SDL_GetPerformanceCounter();

    return (CinderTimer){
        .start = now,
        .last = now,
        .repeat = false};
}

void CinderResetTimer(CinderTimer *t)
{
    if (!t)
        return;

    uint64_t now = SDL_GetPerformanceCounter();

    t->start = now;
    t->last = now;
}

float CinderGetTimerElapsed(const CinderTimer *t)
{
    if (!t)
        return 0.0f;

    uint64_t now = SDL_GetPerformanceCounter();

    return (float)((double)(now - t->start) / (double)gCinderCtx.perfFrequency);
}

bool CinderTimerDone(CinderTimer *t, float duration)
{
    if (!t)
        return false;

    uint64_t now = SDL_GetPerformanceCounter();

    float elapsed = (float)((double)(now - t->last) / (double)gCinderCtx.perfFrequency);

    if (elapsed >= duration)
    {
        if (t->repeat)
            t->last = now;

        return true;
    }

    return false;
}

void CinderTimerSetRepeat(CinderTimer *t, bool repeat)
{
    if (!t)
        return;

    t->repeat = repeat;
}

float CinderGetDeltaTime(void)
{
    return gCinderCtx.deltaTime;
}

void CinderSetTargetFPS(int fps)
{
    gCinderCtx.targetFPS = fps;
}

float CinderGetFPS(void)
{
    return gCinderCtx.fps;
}