#include "cinder_internal.h"

bool CinderIsRunning(void)
{
    return gCinderCtx.loop.isRunning;
}

void CinderRequestQuit(void)
{
    gCinderCtx.loop.isRunning = false;
}

void CinderBeginFrame(void)
{
    // ---------------- Delta time ----------------

    uint64_t now = SDL_GetPerformanceCounter();

    gCinderCtx.timing.deltaTime = (gCinderCtx.timing.lastCounter == 0)
                                      ? 0.0f
                                      : (float)(now - gCinderCtx.timing.lastCounter) / (float)gCinderCtx.timing.perfFrequency;

    gCinderCtx.timing.lastCounter = now;

    if (gCinderCtx.timing.deltaTime > 0.0f)
    {
        float instantFPS = 1.0f / gCinderCtx.timing.deltaTime;
        const float alpha = 0.1f;

        gCinderCtx.timing.fps = (gCinderCtx.timing.fps == 0.0f)
                                    ? instantFPS
                                    : alpha * instantFPS + (1.0f - alpha) * gCinderCtx.timing.fps;
    }

    // ---------------- Reset per-frame input ----------------

    memset(gCinderCtx.input.keyboard.pressed, 0, sizeof(gCinderCtx.input.keyboard.pressed));
    memset(gCinderCtx.input.keyboard.released, 0, sizeof(gCinderCtx.input.keyboard.released));
    memset(gCinderCtx.input.mouse.pressed, 0, sizeof(gCinderCtx.input.mouse.pressed));
    memset(gCinderCtx.input.mouse.released, 0, sizeof(gCinderCtx.input.mouse.released));

    gCinderCtx.input.mouse.delta = (CinderVec2i){{0, 0}};
    gCinderCtx.input.mouse.scrollDelta = (CinderVec2i){{0, 0}};
    gCinderCtx.input.mouse.prevPos = gCinderCtx.input.mouse.pos;
    gCinderCtx.loop.frameBegun = true;

    // ---------------- Event processing ----------------

    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent))
    {
        if (sdlEvent.type == SDL_QUIT)
            CinderRequestQuit();
        else
            CinderInputProcessEvent(&sdlEvent);
    }
}

void CinderEndFrame(void)
{
    if (!gCinderCtx.loop.frameBegun)
    {
        CINDER_LOG(CINDER_LOG_WARNING, "CinderEndFrame called without CinderBeginFrame");
        return;
    }

    gCinderCtx.loop.frameBegun = false;

    if (gCinderCtx.timing.targetFPS > 0)
    {
        double frameTimeSec =
            (double)(SDL_GetPerformanceCounter() - gCinderCtx.timing.lastCounter) /
            (double)gCinderCtx.timing.perfFrequency;

        double targetFrameTime = 1.0 / (double)gCinderCtx.timing.targetFPS;

        if (frameTimeSec < targetFrameTime)
            SDL_Delay((Uint32)((targetFrameTime - frameTimeSec) * 1000.0));
    }

    if (gCinderCtx.core.renderer)
        SDL_RenderPresent(gCinderCtx.core.renderer);
    else
        CINDER_LOG(CINDER_LOG_WARNING, "Renderer is NULL in CinderEndFrame");
}