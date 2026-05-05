#include "cinder_internal.h"

CinderCtx gCinderCtx;

static void CinderRollbackSubsystems(uint32_t flags)
{
    if (flags & SDL_INIT_VIDEO)
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    if (flags & SDL_INIT_AUDIO)
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    if (flags & SDL_INIT_EVENTS)
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

CinderStatus CinderInit(CinderSubsystem flags)
{
    gCinderCtx = (CinderCtx){0};

    gCinderCtx.timing.perfFrequency = SDL_GetPerformanceFrequency();

    CinderInputInit();
    CinderRngSeedAuto(&gCinderCtx.rng);

    if (!gCinderCtx.log.callback)
        CinderSetErrorCallback(CinderDefaultLogCallback, NULL);

    if (flags & CINDER_SUBSYSTEM_EVENTS)
    {
        if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
        {
            CINDER_LOG(CINDER_LOG_ERROR, "Failed to initialize EVENTS subsystem");
            CinderRollbackSubsystems(gCinderCtx.core.initFlags);
            gCinderCtx.core.initFlags = 0;
            return CINDER_STATUS_SUBSYSTEM_INIT_FAILED;
        }
        gCinderCtx.core.initFlags |= SDL_INIT_EVENTS;
    }

    if (flags & CINDER_SUBSYSTEM_AUDIO)
    {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
        {
            CINDER_LOG(CINDER_LOG_ERROR, "Failed to initialize AUDIO subsystem");
            CinderRollbackSubsystems(gCinderCtx.core.initFlags);
            gCinderCtx.core.initFlags = 0;
            return CINDER_STATUS_SUBSYSTEM_INIT_FAILED;
        }
        gCinderCtx.core.initFlags |= SDL_INIT_AUDIO;
    }

    if (flags & CINDER_SUBSYSTEM_VIDEO)
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            CINDER_LOG(CINDER_LOG_ERROR, "Failed to initialize VIDEO subsystem");
            CinderRollbackSubsystems(gCinderCtx.core.initFlags);
            gCinderCtx.core.initFlags = 0;
            return CINDER_STATUS_SUBSYSTEM_INIT_FAILED;
        }
        gCinderCtx.core.initFlags |= SDL_INIT_VIDEO;
    }

    return CINDER_STATUS_OK;
}

void CinderQuit(void)
{
    for (int i = gCinderCtx.pluginCount - 1; i >= 0; i--)
        if (gCinderCtx.plugins[i].shutdown)
            gCinderCtx.plugins[i].shutdown(gCinderCtx.plugins[i].userdata);

    CinderDestroyWindow();

    gCinderCtx.core.initFlags = 0;

    if (gCinderCtx.core.imgInitialized)
        IMG_Quit();
    if (gCinderCtx.core.ttfInitialized)
        TTF_Quit();

    SDL_Quit();
}
