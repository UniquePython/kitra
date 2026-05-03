#include "cinder_internal.h"

bool CinderIsRunning(void)
{
    return gCinderCtx.isRunning;
}

void CinderRequestQuit(void)
{
    gCinderCtx.isRunning = false;
}

void CinderBeginFrame(void)
{
    // ---------------- Delta time ----------------

    uint64_t now = SDL_GetPerformanceCounter();

    gCinderCtx.deltaTime = (gCinderCtx.lastCounter == 0)
                               ? 0.0f
                               : (float)(now - gCinderCtx.lastCounter) / (float)gCinderCtx.perfFrequency;

    gCinderCtx.lastCounter = now;

    if (gCinderCtx.deltaTime > 0.0f)
    {
        float instantFPS = 1.0f / gCinderCtx.deltaTime;
        const float alpha = 0.1f;

        gCinderCtx.fps = (gCinderCtx.fps == 0.0f)
                             ? instantFPS
                             : alpha * instantFPS + (1.0f - alpha) * gCinderCtx.fps;
    }

    // ---------------- Reset per-frame input ----------------

    memset(gCinderCtx.keysPressed, 0, sizeof(gCinderCtx.keysPressed));
    memset(gCinderCtx.mousePressed, 0, sizeof(gCinderCtx.mousePressed));

    gCinderCtx.mouseDelta = (CinderVec2i){{0, 0}};
    gCinderCtx.scrollDelta = (CinderVec2i){{0, 0}};
    gCinderCtx.prevMousePos = gCinderCtx.mousePos;
    gCinderCtx.frameBegun = true;

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
    if (!gCinderCtx.frameBegun)
    {
        CINDER_LOG(CINDER_LOG_WARNING, "CinderEndFrame called without CinderBeginFrame");
        return;
    }

    gCinderCtx.frameBegun = false;

    if (gCinderCtx.targetFPS > 0)
    {
        double frameTimeSec =
            (double)(SDL_GetPerformanceCounter() - gCinderCtx.lastCounter) /
            (double)gCinderCtx.perfFrequency;

        double targetFrameTime = 1.0 / (double)gCinderCtx.targetFPS;

        if (frameTimeSec < targetFrameTime)
            SDL_Delay((Uint32)((targetFrameTime - frameTimeSec) * 1000.0));
    }

    if (gCinderCtx.renderer)
        SDL_RenderPresent(gCinderCtx.renderer);
    else
        CINDER_LOG(CINDER_LOG_WARNING, "Renderer is NULL in CinderEndFrame");
}