#include "cinder.h"

#include <SDL2/SDL.h>

typedef struct CinderCtx
{
    uint32_t initFlags;

    SDL_Window *window;
    SDL_Renderer *renderer;

    bool isRunning;
} CinderCtx;

CinderCtx gCinderCtx;
const char *gCinderErrMsg = "";

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
    gCinderCtx.initFlags = CINDER_SUBSYSTEM_NONE;

    gCinderCtx.window = NULL;
    gCinderCtx.renderer = NULL;

    if (flags & CINDER_SUBSYSTEM_EVENTS)
    {
        if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
        {
            gCinderErrMsg = "Failed to initialize EVENTS subsystem";

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
            gCinderErrMsg = "Failed to initialize AUDIO subsystem";

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
            gCinderErrMsg = "Failed to initialize VIDEO subsystem";

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
    gCinderCtx.initFlags = 0;

    SDL_Quit();
}

CinderStatus CinderCreateWindow(const char *title, size_t width, size_t height)
{
}

CinderStatus CinderDestroyWindow(void)
{
}

const char *CinderGetError(void)
{
    return gCinderErrMsg;
}
