#include "kitra_internal.h"

struct KitraSurface
{
    SDL_Surface *handle;
    int width, height;
};

KitraSurface *KitraCreateSurface(int w, int h)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surface)
    {
        KITRA_LOG(KITRA_LOG_ERROR, SDL_GetError());
        return NULL;
    }

    KitraSurface *s = malloc(sizeof(KitraSurface));
    if (!s)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Failed to allocate KitraSurface");
        SDL_FreeSurface(surface);
        return NULL;
    }

    s->handle = surface;
    s->width = w;
    s->height = h;
    return s;
}

KitraSurface *KitraLoadSurface(const char *path)
{
    if (!gKitraCtx.core.imgInitialized)
    {
        int flags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP | IMG_INIT_TIF;
        int impFormats = IMG_INIT_PNG | IMG_INIT_JPG;

        if ((IMG_Init(flags) & impFormats) != impFormats)
        {
            KITRA_LOG(KITRA_LOG_ERROR, IMG_GetError());
            return NULL;
        }
        gKitraCtx.core.imgInitialized = true;
    }

    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        KITRA_LOG(KITRA_LOG_ERROR, IMG_GetError());
        return NULL;
    }

    SDL_Surface *converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surface);

    if (!converted)
    {
        KITRA_LOG(KITRA_LOG_ERROR, SDL_GetError());
        return NULL;
    }

    KitraSurface *s = malloc(sizeof(KitraSurface));
    if (!s)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Failed to allocate KitraSurface");
        SDL_FreeSurface(converted);
        return NULL;
    }

    s->handle = converted;
    s->width = converted->w;
    s->height = converted->h;
    return s;
}

void KitraDestroySurface(KitraSurface **surface)
{
    if (!surface || !*surface)
        return;

    SDL_FreeSurface((*surface)->handle);
    free(*surface);
    *surface = NULL;
}

KitraTexture *KitraSurfaceToTexture(KitraSurface *surface)
{
    if (!surface)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Surface is NULL in KitraSurfaceToTexture");
        return NULL;
    }

    if (!gKitraCtx.core.renderer)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Renderer not initialized");
        return NULL;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(gKitraCtx.core.renderer, surface->handle);
    if (!tex)
    {
        KITRA_LOG(KITRA_LOG_ERROR, SDL_GetError());
        return NULL;
    }

    KitraTexture *t = KitraTextureFromSDL(tex, surface->width, surface->height);
    if (!t)
    {
        SDL_DestroyTexture(tex);
        return NULL;
    }

    return t;
}

KitraColor KitraReadPixel(KitraSurface *surface, int x, int y)
{
    if (!surface || x < 0 || y < 0 || x >= surface->width || y >= surface->height)
        return (KitraColor){0, 0, 0, 0};

    SDL_LockSurface(surface->handle);
    uint8_t *pixels = (uint8_t *)surface->handle->pixels;
    uint8_t *p = pixels + y * surface->handle->pitch + x * 4;
    KitraColor color = {p[0], p[1], p[2], p[3]};
    SDL_UnlockSurface(surface->handle);

    return color;
}

KitraColor KitraReadPixelP(KitraSurface *surface, KitraPoint point)
{
    return KitraReadPixel(surface, point.x, point.y);
}

void KitraWritePixel(KitraSurface *surface, int x, int y, KitraColor color)
{
    if (!surface || x < 0 || y < 0 || x >= surface->width || y >= surface->height)
        return;

    SDL_LockSurface(surface->handle);
    uint8_t *pixels = (uint8_t *)surface->handle->pixels;
    uint8_t *p = pixels + y * surface->handle->pitch + x * 4;
    p[0] = color.r;
    p[1] = color.g;
    p[2] = color.b;
    p[3] = color.a;
    SDL_UnlockSurface(surface->handle);
}

void KitraWritePixelP(KitraSurface *surface, KitraPoint point, KitraColor color)
{
    KitraWritePixel(surface, point.x, point.y, color);
}

KitraSize KitraGetSurfaceSize(KitraSurface *surface)
{
    if (!surface)
    {
        KITRA_LOG(KITRA_LOG_WARNING, "Surface is NULL in KitraGetSurfaceSize");
        return (KitraSize){{0, 0}};
    }

    return (KitraSize){{surface->width, surface->height}};
}

SDL_Surface *KitraSurfaceGetSDL(KitraSurface *surface)
{
    return surface->handle;
}

KitraSurface *KitraSurfaceFromSDL(SDL_Surface *surface)
{
    KitraSurface *s = malloc(sizeof(KitraSurface));
    if (!s)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Failed to allocate KitraSurface");
        return NULL;
    }

    s->handle = surface;
    s->width = surface->w;
    s->height = surface->h;
    return s;
}