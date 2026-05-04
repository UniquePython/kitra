#include "cinder_internal.h"

#define CINDER_COLOR_UNPACK(c) (c).r, (c).g, (c).b, (c).a

static SDL_Rect CinderToSDLRect(CinderRect rect)
{
    return (SDL_Rect){rect.x, rect.y, rect.w, rect.h};
}

void CinderClearBackground(CinderColor color)
{
    if (!gCinderCtx.core.renderer)
    {
        CINDER_LOG(CINDER_LOG_WARNING, "Renderer is NULL in CinderClearBackground");
        return;
    }

    SDL_SetRenderDrawColor(gCinderCtx.core.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(gCinderCtx.core.renderer);
}

// --------------------------------------- POINT ---------------------------------------

void CinderDrawPoint(int x, int y, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    SDL_SetRenderDrawColor(gCinderCtx.core.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(gCinderCtx.core.renderer, x, y);
}

void CinderDrawPointP(CinderPoint pos, CinderColor color)
{
    CinderDrawPoint(pos.x, pos.y, color);
}

// --------------------------------------- LINE ---------------------------------------

void CinderDrawLine(int x1, int y1, int x2, int y2, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    lineRGBA(gCinderCtx.core.renderer, x1, y1, x2, y2, CINDER_COLOR_UNPACK(color));
}

void CinderDrawLineP(CinderPoint start, CinderPoint end, CinderColor color)
{
    CinderDrawLine(start.x, start.y, end.x, end.y, color);
}

void CinderDrawLineThick(int x1, int y1, int x2, int y2, int width, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    thickLineRGBA(gCinderCtx.core.renderer, x1, y1, x2, y2, width, CINDER_COLOR_UNPACK(color));
}

void CinderDrawLineThickP(CinderPoint start, CinderPoint end, int width, CinderColor color)
{
    CinderDrawLineThick(start.x, start.y, end.x, end.y, width, color);
}

// --------------------------------------- CIRCLE ---------------------------------------

void CinderDrawCircle(int x, int y, int radius, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    filledCircleRGBA(gCinderCtx.core.renderer, x, y, radius, CINDER_COLOR_UNPACK(color));
}

void CinderDrawCircleC(CinderCircle circle, CinderColor color)
{
    CinderDrawCircle(circle.x, circle.y, circle.radius, color);
}

void CinderDrawCircleOutline(int x, int y, int radius, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    circleRGBA(gCinderCtx.core.renderer, x, y, radius, CINDER_COLOR_UNPACK(color));
}

void CinderDrawCircleOutlineC(CinderCircle circle, CinderColor color)
{
    CinderDrawCircleOutline(circle.x, circle.y, circle.radius, color);
}

// --------------------------------------- ELLIPSE ---------------------------------------

void CinderDrawEllipse(int x, int y, int rx, int ry, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    filledEllipseRGBA(gCinderCtx.core.renderer, x, y, rx, ry, CINDER_COLOR_UNPACK(color));
}

void CinderDrawEllipseP(CinderPoint pos, CinderPoint radius, CinderColor color)
{
    CinderDrawEllipse(pos.x, pos.y, radius.x, radius.y, color);
}

void CinderDrawEllipseE(CinderEllipse ellipse, CinderColor color)
{
    CinderDrawEllipse(ellipse.x, ellipse.y, ellipse.rx, ellipse.ry, color);
}

void CinderDrawEllipseOutline(int x, int y, int rx, int ry, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    ellipseRGBA(gCinderCtx.core.renderer, x, y, rx, ry, CINDER_COLOR_UNPACK(color));
}

void CinderDrawEllipseOutlineP(CinderPoint pos, CinderPoint radius, CinderColor color)
{
    CinderDrawEllipseOutline(pos.x, pos.y, radius.x, radius.y, color);
}

void CinderDrawEllipseOutlineE(CinderEllipse ellipse, CinderColor color)
{
    CinderDrawEllipseOutline(ellipse.x, ellipse.y, ellipse.rx, ellipse.ry, color);
}

// --------------------------------------- TRIANGLE ---------------------------------------

void CinderDrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    filledTrigonRGBA(gCinderCtx.core.renderer, x1, y1, x2, y2, x3, y3, CINDER_COLOR_UNPACK(color));
}

void CinderDrawTriangleP(CinderPoint p1, CinderPoint p2, CinderPoint p3, CinderColor color)
{
    CinderDrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
}

void CinderDrawTriangleOutline(int x1, int y1, int x2, int y2, int x3, int y3, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    trigonRGBA(gCinderCtx.core.renderer, x1, y1, x2, y2, x3, y3, CINDER_COLOR_UNPACK(color));
}

void CinderDrawTriangleOutlineP(CinderPoint p1, CinderPoint p2, CinderPoint p3, CinderColor color)
{
    CinderDrawTriangleOutline(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
}

// --------------------------------------- RECTANGLE ---------------------------------------

void CinderDrawRect(CinderRect rect, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    SDL_Rect sdlRect = CinderToSDLRect(rect);
    SDL_SetRenderDrawColor(gCinderCtx.core.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(gCinderCtx.core.renderer, &sdlRect);
}

void CinderDrawRectOutline(CinderRect rect, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    SDL_Rect sdlRect = CinderToSDLRect(rect);
    SDL_SetRenderDrawColor(gCinderCtx.core.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(gCinderCtx.core.renderer, &sdlRect);
}

void CinderDrawRoundedRect(CinderRect rect, int radius, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    roundedBoxRGBA(gCinderCtx.core.renderer,
                   rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1,
                   radius, CINDER_COLOR_UNPACK(color));
}

void CinderDrawRoundedRectOutline(CinderRect rect, int radius, CinderColor color)
{
    if (!gCinderCtx.core.renderer)
        return;

    roundedRectangleRGBA(gCinderCtx.core.renderer,
                         rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1,
                         radius, CINDER_COLOR_UNPACK(color));
}