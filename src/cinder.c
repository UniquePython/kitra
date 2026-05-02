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
    CinderDestroyWindow();

    gCinderCtx.initFlags = 0;

    SDL_Quit();
}

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

CinderStatus CinderCreateWindow(const CinderWindowDesc *winDesc)
{
    CinderWindowDesc internalWinDesc = {0};

    if (!winDesc)
    {
        internalWinDesc.title = "Cinder";
        internalWinDesc.width = 900;
        internalWinDesc.height = 600;
        internalWinDesc.x = SDL_WINDOWPOS_CENTERED;
        internalWinDesc.y = SDL_WINDOWPOS_CENTERED;
        internalWinDesc.flags = CINDER_WINDOW_SHOWN | CINDER_WINDOW_RESIZABLE;
    }
    else
    {
        internalWinDesc.title = winDesc->title;
        internalWinDesc.width = winDesc->width;
        internalWinDesc.height = winDesc->height;
        internalWinDesc.x = winDesc->x;
        internalWinDesc.y = winDesc->y;
        internalWinDesc.flags = winDesc->flags;
    }

    gCinderCtx.window = SDL_CreateWindow(
        internalWinDesc.title,
        internalWinDesc.x == CINDER_WINDOW_POS_CENTERED ? SDL_WINDOWPOS_CENTERED : internalWinDesc.x,
        internalWinDesc.y == CINDER_WINDOW_POS_CENTERED ? SDL_WINDOWPOS_CENTERED : internalWinDesc.y,
        (int)internalWinDesc.width, (int)internalWinDesc.height,
        CinderToSDLWindowFlags(internalWinDesc.flags));

    if (!gCinderCtx.window)
    {
        gCinderErrMsg = SDL_GetError();
        return CINDER_STATUS_WINDOW_CREATE_FAILED;
    }

    gCinderCtx.renderer = SDL_CreateRenderer(gCinderCtx.window, -1, SDL_RENDERER_ACCELERATED);

    if (!gCinderCtx.renderer)
    {
        gCinderErrMsg = SDL_GetError();

        SDL_DestroyWindow(gCinderCtx.window);
        gCinderCtx.window = NULL;

        return CINDER_STATUS_RENDERER_CREATE_FAILED;
    }

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

// ======================================= ERROR ================================================

const char *CinderGetError(void)
{
    return gCinderErrMsg;
}
