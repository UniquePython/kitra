#include "cinder_internal.h"

typedef struct CinderTimer
{
    uint64_t start;
    uint64_t last;
    bool repeat;
} CinderTimer;

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

    return (float)((double)(now - t->start) / (double)gCinderCtx.timing.perfFrequency);
}

bool CinderTimerDone(CinderTimer *t, float duration)
{
    if (!t)
        return false;

    uint64_t now = SDL_GetPerformanceCounter();

    float elapsed = (float)((double)(now - t->last) / (double)gCinderCtx.timing.perfFrequency);

    if (elapsed >= duration)
    {
        if (t->repeat)
            t->last += (uint64_t)((double)duration * (double)gCinderCtx.timing.perfFrequency);

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
    return gCinderCtx.timing.deltaTime;
}

void CinderSetTargetFPS(int fps)
{
    gCinderCtx.timing.targetFPS = fps;
}

float CinderGetFPS(void)
{
    return gCinderCtx.timing.fps;
}