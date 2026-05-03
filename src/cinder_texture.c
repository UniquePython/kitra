#include "cinder_internal.h"

struct CinderTexture
{
    SDL_Texture *handle;
    int width, height;
};

CinderTexture *CinderLoadTexture(const char *path)
{
    if (!gCinderCtx.imgInitialized)
    {
        int flags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP | IMG_INIT_TIF | IMG_INIT_JXL | IMG_INIT_AVIF;
        int impFormats = IMG_INIT_PNG | IMG_INIT_JPG;

        if ((IMG_Init(flags) & impFormats) != impFormats)
        {
            CINDER_LOG(CINDER_LOG_ERROR, IMG_GetError());
            return NULL;
        }
        gCinderCtx.imgInitialized = true;
    }

    if (!gCinderCtx.renderer)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Renderer not initialized");
        return NULL;
    }

    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        CINDER_LOG(CINDER_LOG_ERROR, IMG_GetError());
        return NULL;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(gCinderCtx.renderer, surface);
    SDL_FreeSurface(surface);

    if (!tex)
    {
        CINDER_LOG(CINDER_LOG_ERROR, SDL_GetError());
        return NULL;
    }

    CinderTexture *cTex = malloc(sizeof(CinderTexture));
    if (!cTex)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Failed to allocate CinderTexture");
        SDL_DestroyTexture(tex);
        return NULL;
    }

    cTex->handle = tex;
    SDL_QueryTexture(tex, NULL, NULL, &cTex->width, &cTex->height);

    return cTex;
}

void CinderDrawTextureEx(CinderTexture *tex, const CinderRect *src, const CinderRect *dst)
{
    if (!gCinderCtx.renderer || !tex)
        return;

    SDL_Rect sdlSrc, sdlDst;
    SDL_Rect *pSrc = NULL, *pDst = NULL;

    if (src)
    {
        sdlSrc = (SDL_Rect){src->x, src->y, src->w, src->h};
        pSrc = &sdlSrc;
    }

    if (dst)
    {
        sdlDst = (SDL_Rect){dst->x, dst->y, dst->w, dst->h};
        pDst = &sdlDst;
    }

    SDL_RenderCopy(gCinderCtx.renderer, tex->handle, pSrc, pDst);
}

void CinderDrawTexture(CinderTexture *tex, int x, int y)
{
    if (!tex)
        return;

    CinderRect dst = {x, y, tex->width, tex->height};
    CinderDrawTextureEx(tex, NULL, &dst);
}

void CinderDrawTextureV(CinderTexture *tex, CinderPoint pos)
{
    CinderDrawTexture(tex, pos.x, pos.y);
}

void CinderDestroyTexture(CinderTexture **tex)
{
    if (!tex || !*tex)
        return;

    SDL_DestroyTexture((*tex)->handle);
    free(*tex);
    *tex = NULL;
}