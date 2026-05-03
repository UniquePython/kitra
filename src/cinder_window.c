#include "cinder_internal.h"

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
    gCinderCtx.core.window = SDL_CreateWindow(
        winDesc.title,
        winDesc.centerX ? (int)SDL_WINDOWPOS_CENTERED : winDesc.pos.x,
        winDesc.centerY ? (int)SDL_WINDOWPOS_CENTERED : winDesc.pos.y,
        winDesc.size.w, winDesc.size.h,
        CinderToSDLWindowFlags(winDesc.flags));

    if (!gCinderCtx.core.window)
    {
        CINDER_LOG(CINDER_LOG_ERROR, SDL_GetError());
        return CINDER_STATUS_WINDOW_CREATE_FAILED;
    }

    gCinderCtx.core.renderer = SDL_CreateRenderer(gCinderCtx.core.window, -1, SDL_RENDERER_ACCELERATED);

    if (!gCinderCtx.core.renderer)
    {
        CINDER_LOG(CINDER_LOG_ERROR, SDL_GetError());
        SDL_DestroyWindow(gCinderCtx.core.window);
        gCinderCtx.core.window = NULL;
        return CINDER_STATUS_RENDERER_CREATE_FAILED;
    }

    gCinderCtx.loop.isRunning = true;

    return CINDER_STATUS_OK;
}

void CinderDestroyWindow(void)
{
    if (gCinderCtx.core.renderer)
    {
        SDL_DestroyRenderer(gCinderCtx.core.renderer);
        gCinderCtx.core.renderer = NULL;
    }

    if (gCinderCtx.core.window)
    {
        SDL_DestroyWindow(gCinderCtx.core.window);
        gCinderCtx.core.window = NULL;
    }
}
