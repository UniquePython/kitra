#include "cinder.h"

#include <SDL2/SDL.h>

typedef struct CinderCtx
{
    uint32_t initFlags;

    SDL_Window *window;
    SDL_Renderer *renderer;

    bool isRunning;
    bool frameBegun;

    const char *errMsg;

} CinderCtx;

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
    gCinderCtx.initFlags = CINDER_SUBSYSTEM_NONE;

    gCinderCtx.window = NULL;
    gCinderCtx.renderer = NULL;

    if (flags & CINDER_SUBSYSTEM_EVENTS)
    {
        if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
        {
            gCinderCtx.errMsg = "Failed to initialize EVENTS subsystem";

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
            gCinderCtx.errMsg = "Failed to initialize AUDIO subsystem";

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
            gCinderCtx.errMsg = "Failed to initialize VIDEO subsystem";

            CinderRollbackSubsystems(gCinderCtx.initFlags);
            gCinderCtx.initFlags = 0;

            return CINDER_STATUS_SUBSYSTEM_INIT_FAILED;
        }
        gCinderCtx.initFlags |= SDL_INIT_VIDEO;
    }

    return CINDER_STATUS_OK;
}

// Forward declare destruction functions
void CinderDestroyWindow(void);

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
        gCinderCtx.errMsg = SDL_GetError();
        return CINDER_STATUS_WINDOW_CREATE_FAILED;
    }

    gCinderCtx.renderer = SDL_CreateRenderer(gCinderCtx.window, -1, SDL_RENDERER_ACCELERATED);

    if (!gCinderCtx.renderer)
    {
        gCinderCtx.errMsg = SDL_GetError();

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
    SDL_Event sdlEvent;

    gCinderCtx.frameBegun = true;

    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            CinderRequestQuit();
            break;

        default:
            break;
        }
    }
}

void CinderEndFrame(void)
{
    if (!gCinderCtx.frameBegun)
    {
        gCinderCtx.errMsg = "CinderEndFrame called without CinderBeginFrame";
        return;
    }

    gCinderCtx.frameBegun = false;

    if (gCinderCtx.renderer)
    {
        SDL_RenderPresent(gCinderCtx.renderer);
    }
    else
    {
        gCinderCtx.errMsg = "Renderer is NULL in CinderEndFrame";
    }
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

// ======================================= DRAWING ================================================

void CinderClearBackground(CinderColor color)
{
    if (!gCinderCtx.renderer)
    {
        gCinderCtx.errMsg = "Renderer is NULL in CinderClearBackground";
        return;
    }

    SDL_SetRenderDrawColor(gCinderCtx.renderer, color.r, color.g, color.b, color.a);

    SDL_RenderClear(gCinderCtx.renderer);
}

// --------------------------------------- RECTANGLE ---------------------------------------

static SDL_Rect CinderToSDLRect(CinderRect rect)
{
    return (SDL_Rect){rect.x, rect.y, rect.w, rect.h};
}

void CinderDrawRect(CinderRect rect, CinderColor color)
{
    if (!gCinderCtx.renderer)
        return;

    SDL_Rect sdlRect = CinderToSDLRect(rect);

    SDL_SetRenderDrawColor(gCinderCtx.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(gCinderCtx.renderer, &sdlRect);
}

void CinderDrawRectOutline(CinderRect rect, CinderColor color)
{
    if (!gCinderCtx.renderer)
        return;

    SDL_Rect sdlRect = CinderToSDLRect(rect);

    SDL_SetRenderDrawColor(gCinderCtx.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(gCinderCtx.renderer, &sdlRect);
}

// ======================================= ERROR ================================================

const char *CinderGetError(void)
{
    return gCinderCtx.errMsg;
}
