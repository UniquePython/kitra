#include "cinder_internal.h"

// ======================================= WINDOW ================================================

static uint32_t CinderToSDLWindowFlags(CinderWindowFlags flags)
{
    uint32_t sdlFlags = 0;

    if (flags & CINDER_WINDOW_SHOWN)
        sdlFlags |= SDL_WINDOW_SHOWN;
    if (flags & CINDER_WINDOW_RESIZABLE)
        sdlFlags |= SDL_WINDOW_RESIZABLE;
    if (flags & CINDER_WINDOW_FULLSCREEN)
        sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if (flags & CINDER_WINDOW_BORDERLESS)
        sdlFlags |= SDL_WINDOW_BORDERLESS;
    if (flags & CINDER_WINDOW_HIDDEN)
        sdlFlags |= SDL_WINDOW_HIDDEN;

    return sdlFlags;
}

CinderWindowDesc CinderDefaultWindowDesc(void)
{
    return (CinderWindowDesc){
        .title = "Cinder",
        .size = {.w = 900, .h = 600},
        .pos = {.x = 0, .y = 0},
        .centerX = true,
        .centerY = true,
        .flags = CINDER_WINDOW_SHOWN | CINDER_WINDOW_RESIZABLE};
}

CinderStatus CinderCreateWindow(CinderWindowDesc winDesc)
{
    gCinderCtx.window = SDL_CreateWindow(
        winDesc.title,
        winDesc.centerX ? (int)SDL_WINDOWPOS_CENTERED : winDesc.pos.x,
        winDesc.centerY ? (int)SDL_WINDOWPOS_CENTERED : winDesc.pos.y,
        winDesc.size.w, winDesc.size.h,
        CinderToSDLWindowFlags(winDesc.flags));

    if (!gCinderCtx.window)
    {
        CINDER_LOG(CINDER_LOG_ERROR, SDL_GetError());
        return CINDER_STATUS_WINDOW_CREATE_FAILED;
    }

    gCinderCtx.renderer = SDL_CreateRenderer(gCinderCtx.window, -1, SDL_RENDERER_ACCELERATED);

    if (!gCinderCtx.renderer)
    {
        CINDER_LOG(CINDER_LOG_ERROR, SDL_GetError());
        SDL_DestroyWindow(gCinderCtx.window);
        gCinderCtx.window = NULL;
        return CINDER_STATUS_RENDERER_CREATE_FAILED;
    }

    gCinderCtx.isRunning = true;

    return CINDER_STATUS_OK;
}

void CinderDestroyWindow(void)
{
    if (gCinderCtx.renderer)
    {
        SDL_DestroyRenderer(gCinderCtx.renderer);
        gCinderCtx.renderer = NULL;
    }

    if (gCinderCtx.window)
    {
        SDL_DestroyWindow(gCinderCtx.window);
        gCinderCtx.window = NULL;
    }
}

// ======================================= CORE LOOP ================================================

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