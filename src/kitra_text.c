#include "kitra_internal.h"

#include <stdarg.h>

struct KitraFont
{
    TTF_Font *handle;
    int size;
};

struct KitraText
{
    SDL_Texture *handle;
    KitraSize size;
};

KitraFont *KitraLoadFont(const char *path, int size)
{
    if (!path || size <= 0)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Invalid font path or size");
        return NULL;
    }

    if (!gKitraCtx.core.ttfInitialized)
    {
        if (TTF_Init() < 0)
        {
            KITRA_LOG(KITRA_LOG_ERROR, TTF_GetError());
            return NULL;
        }
        gKitraCtx.core.ttfInitialized = true;
    }

    TTF_Font *font = TTF_OpenFont(path, size);
    if (!font)
    {
        KITRA_LOG(KITRA_LOG_ERROR, TTF_GetError());
        return NULL;
    }

    KitraFont *cfont = malloc(sizeof(KitraFont));
    if (!cfont)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Failed to allocate KitraFont");
        TTF_CloseFont(font);
        return NULL;
    }

    cfont->handle = font;
    cfont->size = size;

    return cfont;
}

void KitraDestroyFont(KitraFont **font)
{
    if (!font || !*font)
        return;

    TTF_CloseFont((*font)->handle);
    free(*font);
    *font = NULL;
}

void KitraDrawText(KitraFont *font, const char *text, int x, int y, KitraColor color)
{
    if (!gKitraCtx.core.renderer || !font || !text)
        return;

    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font->handle, text, sdlColor);
    if (!surface)
    {
        KITRA_LOG(KITRA_LOG_ERROR, TTF_GetError());
        return;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(gKitraCtx.core.renderer, surface);
    SDL_FreeSurface(surface);

    if (!tex)
    {
        KITRA_LOG(KITRA_LOG_ERROR, SDL_GetError());
        return;
    }

    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    SDL_RenderCopy(gKitraCtx.core.renderer, tex, NULL, &(SDL_Rect){x, y, w, h});
    SDL_DestroyTexture(tex);
}

void KitraDrawTextP(KitraFont *font, const char *text, KitraPoint pos, KitraColor color)
{
    KitraDrawText(font, text, pos.x, pos.y, color);
}

void KitraDrawTextF(KitraFont *font, int x, int y, KitraColor color, const char *fmt, ...)
{
    va_list args, args2;
    va_start(args, fmt);
    va_copy(args2, args);

    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (len < 0)
    {
        va_end(args2);
        return;
    }

    char *buf = malloc((size_t)len + 1);
    if (!buf)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Failed to allocate text buffer");
        va_end(args2);
        return;
    }

    vsnprintf(buf, (size_t)len + 1, fmt, args2);
    va_end(args2);

    KitraDrawText(font, buf, x, y, color);
    free(buf);
}

void KitraDrawTextFP(KitraFont *font, KitraPoint pos, KitraColor color, const char *fmt, ...)
{
    va_list args, args2;
    va_start(args, fmt);
    va_copy(args2, args);

    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (len < 0)
    {
        va_end(args2);
        return;
    }

    char *buf = malloc((size_t)len + 1);
    if (!buf)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Failed to allocate text buffer");
        va_end(args2);
        return;
    }

    vsnprintf(buf, (size_t)len + 1, fmt, args2);
    va_end(args2);

    KitraDrawText(font, buf, pos.x, pos.y, color);
    free(buf);
}

KitraText *KitraCreateText(KitraFont *font, const char *text, KitraColor color)
{
    if (!gKitraCtx.core.renderer || !font || !text)
        return NULL;

    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font->handle, text, sdlColor);
    if (!surface)
    {
        KITRA_LOG(KITRA_LOG_ERROR, TTF_GetError());
        return NULL;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(gKitraCtx.core.renderer, surface);
    SDL_FreeSurface(surface);

    if (!tex)
    {
        KITRA_LOG(KITRA_LOG_ERROR, SDL_GetError());
        return NULL;
    }

    KitraText *ct = malloc(sizeof(KitraText));
    if (!ct)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Failed to allocate KitraText");
        SDL_DestroyTexture(tex);
        return NULL;
    }

    ct->handle = tex;
    SDL_QueryTexture(tex, NULL, NULL, &ct->size.w, &ct->size.h);

    return ct;
}

void KitraDrawCachedText(KitraText *t, int x, int y)
{
    if (!gKitraCtx.core.renderer || !t || !t->handle)
        return;

    SDL_RenderCopy(gKitraCtx.core.renderer, t->handle, NULL, &(SDL_Rect){x, y, t->size.w, t->size.h});
}

void KitraDrawCachedTextP(KitraText *t, KitraPoint pos)
{
    KitraDrawCachedText(t, pos.x, pos.y);
}

void KitraDestroyCachedText(KitraText **t)
{
    if (!t || !*t)
        return;

    SDL_DestroyTexture((*t)->handle);
    free(*t);
    *t = NULL;
}

KitraSize KitraMeasureText(KitraFont *font, const char *text)
{
    KitraSize size = {{0, 0}};

    if (!font || !font->handle || !text)
        return size;

    int w = 0, h = 0;
    if (TTF_SizeUTF8(font->handle, text, &w, &h) != 0)
    {
        KITRA_LOG(KITRA_LOG_ERROR, TTF_GetError());
        return size;
    }

    size.w = w;
    size.h = h;

    return size;
}

KitraSize KitraMeasureTextF(KitraFont *font, const char *fmt, ...)
{
    va_list args, args2;
    va_start(args, fmt);
    va_copy(args2, args);

    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (len < 0)
    {
        va_end(args2);
        return (KitraSize){{0, 0}};
    }

    char *buf = malloc((size_t)len + 1);
    if (!buf)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Failed to allocate text buffer");
        va_end(args2);
        return (KitraSize){{0, 0}};
    }

    vsnprintf(buf, (size_t)len + 1, fmt, args2);
    va_end(args2);

    KitraSize size = KitraMeasureText(font, buf);
    free(buf);
    return size;
}
