#include "cinder_internal.h"

#define CINDER_COLOR_UNPACK(c) (c).r, (c).g, (c).b, (c).a

static SDL_Rect CinderToSDLRect(CinderRect rect)
{
    return (SDL_Rect){rect.x, rect.y, rect.w, rect.h};
}

void CinderClearBackground(CinderColor color)
{
    if (!gCinderCtx.renderer)
    {
        CINDER_LOG(CINDER_LOG_WARNING, "Renderer is NULL in CinderClearBackground");
        return;
    }

    SDL_SetRenderDrawColor(gCinderCtx.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(gCinderCtx.renderer);
}

// --------------------------------------- LINE ---------------------------------------

void CinderDrawLine(int x1, int y1, int x2, int y2, CinderColor color)
{
    if (!gCinderCtx.renderer)
        return;

    lineRGBA(gCinderCtx.renderer, x1, y1, x2, y2, CINDER_COLOR_UNPACK(color));
}

void CinderDrawLineV(CinderPoint start, CinderPoint end, CinderColor color)
{
    CinderDrawLine(start.x, start.y, end.x, end.y, color);
}

// --------------------------------------- CIRCLE ---------------------------------------

void CinderDrawCircle(int x, int y, int radius, CinderColor color)
{
    if (!gCinderCtx.renderer)
        return;

    filledCircleRGBA(gCinderCtx.renderer, x, y, radius, CINDER_COLOR_UNPACK(color));
}

void CinderDrawCircleV(CinderPoint pos, int radius, CinderColor color)
{
    CinderDrawCircle(pos.x, pos.y, radius, color);
}

void CinderDrawCircleOutline(int x, int y, int radius, CinderColor color)
{
    if (!gCinderCtx.renderer)
        return;

    circleRGBA(gCinderCtx.renderer, x, y, radius, CINDER_COLOR_UNPACK(color));
}

void CinderDrawCircleOutlineV(CinderPoint pos, int radius, CinderColor color)
{
    CinderDrawCircleOutline(pos.x, pos.y, radius, color);
}

// --------------------------------------- TRIANGLE ---------------------------------------

void CinderDrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, CinderColor color)
{
    if (!gCinderCtx.renderer)
        return;

    filledTrigonRGBA(gCinderCtx.renderer, x1, y1, x2, y2, x3, y3, CINDER_COLOR_UNPACK(color));
}

void CinderDrawTriangleV(CinderPoint p1, CinderPoint p2, CinderPoint p3, CinderColor color)
{
    CinderDrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
}

void CinderDrawTriangleOutline(int x1, int y1, int x2, int y2, int x3, int y3, CinderColor color)
{
    if (!gCinderCtx.renderer)
        return;

    trigonRGBA(gCinderCtx.renderer, x1, y1, x2, y2, x3, y3, CINDER_COLOR_UNPACK(color));
}

void CinderDrawTriangleOutlineV(CinderPoint p1, CinderPoint p2, CinderPoint p3, CinderColor color)
{
    CinderDrawTriangleOutline(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
}

// --------------------------------------- RECTANGLE ---------------------------------------

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
    SDL_RenderDrawRect(gCinderCtx.renderer, &sdlRect);
}

void CinderDrawRoundedRect(CinderRect rect, int radius, CinderColor color)
{
    if (!gCinderCtx.renderer)
        return;

    roundedBoxRGBA(gCinderCtx.renderer,
                   rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1,
                   radius, CINDER_COLOR_UNPACK(color));
}

void CinderDrawRoundedRectOutline(CinderRect rect, int radius, CinderColor color)
{
    if (!gCinderCtx.renderer)
        return;

    roundedRectangleRGBA(gCinderCtx.renderer,
                         rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1,
                         radius, CINDER_COLOR_UNPACK(color));
}