#include "cinder_internal.h"

CinderCtx gCinderCtx;

// ======================================= SUBSYSTEM ================================================

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

    gCinderCtx.perfFrequency = SDL_GetPerformanceFrequency();

    CinderInputInit();

    if (!gCinderCtx.callback)
        CinderSetErrorCallback(CinderDefaultLogCallback, NULL);

    if (flags & CINDER_SUBSYSTEM_EVENTS)
    {
        if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
        {
            CINDER_LOG(CINDER_LOG_ERROR, "Failed to initialize EVENTS subsystem");
            CinderRollbackSubsystems(gCinderCtx.initFlags);
            gCinderCtx.initFlags = 0;
            return CINDER_STATUS_SUBSYSTEM_INIT_FAILED;
        }
        gCinderCtx.initFlags |= SDL_INIT_EVENTS;
    }

    if (flags & CINDER_SUBSYSTEM_AUDIO)
    {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
        {
            CINDER_LOG(CINDER_LOG_ERROR, "Failed to initialize AUDIO subsystem");
            CinderRollbackSubsystems(gCinderCtx.initFlags);
            gCinderCtx.initFlags = 0;
            return CINDER_STATUS_SUBSYSTEM_INIT_FAILED;
        }
        gCinderCtx.initFlags |= SDL_INIT_AUDIO;
    }

    if (flags & CINDER_SUBSYSTEM_VIDEO)
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            CINDER_LOG(CINDER_LOG_ERROR, "Failed to initialize VIDEO subsystem");
            CinderRollbackSubsystems(gCinderCtx.initFlags);
            gCinderCtx.initFlags = 0;
            return CINDER_STATUS_SUBSYSTEM_INIT_FAILED;
        }
        gCinderCtx.initFlags |= SDL_INIT_VIDEO;
    }

    return CINDER_STATUS_OK;
}

void CinderQuit(void)
{
    CinderDestroyWindow();

    gCinderCtx.initFlags = 0;

    if (gCinderCtx.imgInitialized)
        IMG_Quit();
    if (gCinderCtx.ttfInitialized)
        TTF_Quit();

    SDL_Quit();
}

// ======================================= COLOR ================================================

const CinderColor CINDER_BLACK = CINDER_COLOR_RGB(0, 0, 0);
const CinderColor CINDER_WHITE = CINDER_COLOR_RGB(255, 255, 255);
const CinderColor CINDER_RED = CINDER_COLOR_RGB(255, 0, 0);
const CinderColor CINDER_GREEN = CINDER_COLOR_RGB(0, 255, 0);
const CinderColor CINDER_BLUE = CINDER_COLOR_RGB(0, 0, 255);
const CinderColor CINDER_YELLOW = CINDER_COLOR_RGB(255, 255, 0);
const CinderColor CINDER_MAGENTA = CINDER_COLOR_RGB(255, 0, 255);
const CinderColor CINDER_CYAN = CINDER_COLOR_RGB(0, 255, 255);