#include "kitra_internal.h"

static uint32_t KitraToSDLWindowFlags(KitraWindowFlags flags)
{
    uint32_t sdlFlags = 0;

    if (flags & KITRA_WINDOW_SHOWN)
        sdlFlags |= SDL_WINDOW_SHOWN;
    if (flags & KITRA_WINDOW_RESIZABLE)
        sdlFlags |= SDL_WINDOW_RESIZABLE;
    if (flags & KITRA_WINDOW_FULLSCREEN)
        sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if (flags & KITRA_WINDOW_BORDERLESS)
        sdlFlags |= SDL_WINDOW_BORDERLESS;
    if (flags & KITRA_WINDOW_HIDDEN)
        sdlFlags |= SDL_WINDOW_HIDDEN;

    return sdlFlags;
}

KitraWindowDesc KitraDefaultWindowDesc(void)
{
    return (KitraWindowDesc){
        .title = "Kitra",
        .size = {.w = 900, .h = 600},
        .pos = {.x = 0, .y = 0},
        .centerX = true,
        .centerY = true,
        .flags = KITRA_WINDOW_SHOWN | KITRA_WINDOW_RESIZABLE};
}

KitraStatus KitraCreateWindow(KitraWindowDesc winDesc)
{
    gKitraCtx.core.window = SDL_CreateWindow(
        winDesc.title,
        winDesc.centerX ? (int)SDL_WINDOWPOS_CENTERED : winDesc.pos.x,
        winDesc.centerY ? (int)SDL_WINDOWPOS_CENTERED : winDesc.pos.y,
        winDesc.size.w, winDesc.size.h,
        KitraToSDLWindowFlags(winDesc.flags));

    if (!gKitraCtx.core.window)
    {
        KITRA_LOG(KITRA_LOG_ERROR, SDL_GetError());
        return KITRA_STATUS_WINDOW_CREATE_FAILED;
    }

    gKitraCtx.core.renderer = SDL_CreateRenderer(gKitraCtx.core.window, -1, SDL_RENDERER_ACCELERATED);

    if (!gKitraCtx.core.renderer)
    {
        KITRA_LOG(KITRA_LOG_ERROR, SDL_GetError());
        SDL_DestroyWindow(gKitraCtx.core.window);
        gKitraCtx.core.window = NULL;
        return KITRA_STATUS_RENDERER_CREATE_FAILED;
    }

    gKitraCtx.loop.isRunning = true;

    return KITRA_STATUS_OK;
}

void KitraDestroyWindow(void)
{
    if (gKitraCtx.core.renderer)
    {
        SDL_DestroyRenderer(gKitraCtx.core.renderer);
        gKitraCtx.core.renderer = NULL;
    }

    if (gKitraCtx.core.window)
    {
        SDL_DestroyWindow(gKitraCtx.core.window);
        gKitraCtx.core.window = NULL;
    }
}

KitraSize KitraGetWindowSize(void)
{
    KitraSize size = {{0, 0}};

    if (!gKitraCtx.core.window)
    {
        KITRA_LOG(KITRA_LOG_WARNING, "Window is NULL in KitraGetWindowSize");
        return size;
    }

    SDL_GetWindowSize(gKitraCtx.core.window, &size.w, &size.h);
    return size;
}

bool KitraIsWindowResized(void)
{
    return gKitraCtx.loop.resized;
}

// Backdoor functions

struct SDL_Window *KitraGetSDLWindow(void)
{
    return gKitraCtx.core.window;
}

struct SDL_Renderer *KitraGetSDLRenderer(void)
{
    return gKitraCtx.core.renderer;
}
