#include "cinder_internal.h"

struct CinderFont
{
    TTF_Font *handle;
    int size;
};

struct CinderText
{
    SDL_Texture *handle;
    int width, height;

    CinderFont *font;
    char *text;
    CinderColor color;
};

CinderFont *CinderLoadFont(const char *path, int size)
{
    if (!path || size <= 0)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Invalid font path or size");
        return NULL;
    }

    if (!gCinderCtx.core.ttfInitialized)
    {
        if (TTF_Init() < 0)
        {
            CINDER_LOG(CINDER_LOG_ERROR, TTF_GetError());
            return NULL;
        }
        gCinderCtx.core.ttfInitialized = true;
    }

    TTF_Font *font = TTF_OpenFont(path, size);
    if (!font)
    {
        CINDER_LOG(CINDER_LOG_ERROR, TTF_GetError());
        return NULL;
    }

    CinderFont *cfont = malloc(sizeof(CinderFont));
    if (!cfont)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Failed to allocate CinderFont");
        TTF_CloseFont(font);
        return NULL;
    }

    cfont->handle = font;
    cfont->size = size;

    return cfont;
}

void CinderDestroyFont(CinderFont **font)
{
    if (!font || !*font)
        return;

    TTF_CloseFont((*font)->handle);
    free(*font);
    *font = NULL;
}

void CinderDrawText(CinderFont *font, const char *text, int x, int y, CinderColor color)
{
    if (!gCinderCtx.core.renderer || !font || !text)
        return;

    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font->handle, text, sdlColor);
    if (!surface)
    {
        CINDER_LOG(CINDER_LOG_ERROR, TTF_GetError());
        return;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(gCinderCtx.core.renderer, surface);
    SDL_FreeSurface(surface);

    if (!tex)
    {
        CINDER_LOG(CINDER_LOG_ERROR, SDL_GetError());
        return;
    }

    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    SDL_RenderCopy(gCinderCtx.core.renderer, tex, NULL, &(SDL_Rect){x, y, w, h});
    SDL_DestroyTexture(tex);
}

void CinderDrawTextV(CinderFont *font, const char *text, CinderPoint pos, CinderColor color)
{
    CinderDrawText(font, text, pos.x, pos.y, color);
}

CinderText *CinderCreateText(CinderFont *font, const char *text, CinderColor color)
{
    if (!gCinderCtx.core.renderer || !font || !text)
        return NULL;

    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font->handle, text, sdlColor);
    if (!surface)
    {
        CINDER_LOG(CINDER_LOG_ERROR, TTF_GetError());
        return NULL;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(gCinderCtx.core.renderer, surface);
    SDL_FreeSurface(surface);

    if (!tex)
    {
        CINDER_LOG(CINDER_LOG_ERROR, SDL_GetError());
        return NULL;
    }

    CinderText *ct = malloc(sizeof(CinderText));
    if (!ct)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Failed to allocate CinderText");
        SDL_DestroyTexture(tex);
        return NULL;
    }

    ct->handle = tex;
    SDL_QueryTexture(tex, NULL, NULL, &ct->width, &ct->height);
    ct->font = font;
    ct->text = strdup(text);
    ct->color = color;

    return ct;
}

void CinderDrawCachedText(CinderText *t, int x, int y)
{
    if (!gCinderCtx.core.renderer || !t || !t->handle)
        return;

    SDL_RenderCopy(gCinderCtx.core.renderer, t->handle, NULL, &(SDL_Rect){x, y, t->width, t->height});
}

void CinderDrawCachedTextV(CinderText *t, CinderPoint pos)
{
    CinderDrawCachedText(t, pos.x, pos.y);
}

void CinderDestroyCachedText(CinderText **t)
{
    if (!t || !*t)
        return;

    SDL_DestroyTexture((*t)->handle);
    free((*t)->text);
    free(*t);
    *t = NULL;
}

CinderSize CinderMeasureText(CinderFont *font, const char *text)
{
    CinderSize size = {{0, 0}};

    if (!font || !font->handle || !text)
        return size;

    int w = 0, h = 0;
    if (TTF_SizeUTF8(font->handle, text, &w, &h) != 0)
    {
        CINDER_LOG(CINDER_LOG_ERROR, TTF_GetError());
        return size;
    }

    size.w = w;
    size.h = h;

    return size;
}