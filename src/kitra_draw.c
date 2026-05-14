#include "kitra_internal.h"

#define KITRA_COLOR_UNPACK(c) (c).r, (c).g, (c).b, (c).a

static SDL_Rect KitraToSDLRect(KitraRect rect)
{
    return (SDL_Rect){rect.x, rect.y, rect.w, rect.h};
}

void KitraClearBackground(KitraColor color)
{
    if (!gKitraCtx.core.renderer)
    {
        KITRA_LOG(KITRA_LOG_WARNING, "Renderer is NULL in KitraClearBackground");
        return;
    }

    SDL_SetRenderDrawColor(gKitraCtx.core.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(gKitraCtx.core.renderer);
}

// --------------------------------------- POINT ---------------------------------------

void KitraDrawPoint(int x, int y, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    SDL_SetRenderDrawColor(gKitraCtx.core.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(gKitraCtx.core.renderer, x, y);
}

void KitraDrawPointP(KitraPoint pos, KitraColor color)
{
    KitraDrawPoint(pos.x, pos.y, color);
}

// --------------------------------------- LINE ---------------------------------------

void KitraDrawLine(int x1, int y1, int x2, int y2, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    lineRGBA(gKitraCtx.core.renderer, x1, y1, x2, y2, KITRA_COLOR_UNPACK(color));
}

void KitraDrawLineP(KitraPoint start, KitraPoint end, KitraColor color)
{
    KitraDrawLine(start.x, start.y, end.x, end.y, color);
}

void KitraDrawLineThick(int x1, int y1, int x2, int y2, int width, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    thickLineRGBA(gKitraCtx.core.renderer, x1, y1, x2, y2, width, KITRA_COLOR_UNPACK(color));
}

void KitraDrawLineThickP(KitraPoint start, KitraPoint end, int width, KitraColor color)
{
    KitraDrawLineThick(start.x, start.y, end.x, end.y, width, color);
}

// --------------------------------------- CIRCLE ---------------------------------------

void KitraDrawCircle(int x, int y, int radius, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    filledCircleRGBA(gKitraCtx.core.renderer, x, y, radius, KITRA_COLOR_UNPACK(color));
}

void KitraDrawCircleC(KitraCircle circle, KitraColor color)
{
    KitraDrawCircle(circle.x, circle.y, circle.radius, color);
}

void KitraDrawCircleOutline(int x, int y, int radius, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    circleRGBA(gKitraCtx.core.renderer, x, y, radius, KITRA_COLOR_UNPACK(color));
}

void KitraDrawCircleOutlineC(KitraCircle circle, KitraColor color)
{
    KitraDrawCircleOutline(circle.x, circle.y, circle.radius, color);
}

// --------------------------------------- ELLIPSE ---------------------------------------

void KitraDrawEllipse(int x, int y, int rx, int ry, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    filledEllipseRGBA(gKitraCtx.core.renderer, x, y, rx, ry, KITRA_COLOR_UNPACK(color));
}

void KitraDrawEllipseP(KitraPoint pos, KitraPoint radius, KitraColor color)
{
    KitraDrawEllipse(pos.x, pos.y, radius.x, radius.y, color);
}

void KitraDrawEllipseE(KitraEllipse ellipse, KitraColor color)
{
    KitraDrawEllipse(ellipse.x, ellipse.y, ellipse.rx, ellipse.ry, color);
}

void KitraDrawEllipseOutline(int x, int y, int rx, int ry, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    ellipseRGBA(gKitraCtx.core.renderer, x, y, rx, ry, KITRA_COLOR_UNPACK(color));
}

void KitraDrawEllipseOutlineP(KitraPoint pos, KitraPoint radius, KitraColor color)
{
    KitraDrawEllipseOutline(pos.x, pos.y, radius.x, radius.y, color);
}

void KitraDrawEllipseOutlineE(KitraEllipse ellipse, KitraColor color)
{
    KitraDrawEllipseOutline(ellipse.x, ellipse.y, ellipse.rx, ellipse.ry, color);
}

// --------------------------------------- TRIANGLE ---------------------------------------

void KitraDrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    filledTrigonRGBA(gKitraCtx.core.renderer, x1, y1, x2, y2, x3, y3, KITRA_COLOR_UNPACK(color));
}

void KitraDrawTriangleP(KitraPoint p1, KitraPoint p2, KitraPoint p3, KitraColor color)
{
    KitraDrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
}

void KitraDrawTriangleOutline(int x1, int y1, int x2, int y2, int x3, int y3, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    trigonRGBA(gKitraCtx.core.renderer, x1, y1, x2, y2, x3, y3, KITRA_COLOR_UNPACK(color));
}

void KitraDrawTriangleOutlineP(KitraPoint p1, KitraPoint p2, KitraPoint p3, KitraColor color)
{
    KitraDrawTriangleOutline(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
}

// --------------------------------------- POLYGON ---------------------------------------

void KitraDrawPolygon(const KitraPoint *points, int count, KitraColor color)
{
    if (!gKitraCtx.core.renderer || !points || count < 3)
        return;

    int16_t *xs = malloc(sizeof(int16_t) * (size_t)count);
    int16_t *ys = malloc(sizeof(int16_t) * (size_t)count);

    if (!xs || !ys)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Failed to allocate polygon buffers");
        free(xs);
        free(ys);
        return;
    }

    for (int i = 0; i < count; i++)
    {
        xs[i] = (int16_t)points[i].x;
        ys[i] = (int16_t)points[i].y;
    }

    filledPolygonRGBA(gKitraCtx.core.renderer, xs, ys, count, KITRA_COLOR_UNPACK(color));

    free(xs);
    free(ys);
}

void KitraDrawPolygonOutline(const KitraPoint *points, int count, KitraColor color)
{
    if (!gKitraCtx.core.renderer || !points || count < 3)
        return;

    int16_t *xs = malloc(sizeof(int16_t) * (size_t)count);
    int16_t *ys = malloc(sizeof(int16_t) * (size_t)count);

    if (!xs || !ys)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Failed to allocate polygon buffers");
        free(xs);
        free(ys);
        return;
    }

    for (int i = 0; i < count; i++)
    {
        xs[i] = (int16_t)points[i].x;
        ys[i] = (int16_t)points[i].y;
    }

    polygonRGBA(gKitraCtx.core.renderer, xs, ys, count, KITRA_COLOR_UNPACK(color));

    free(xs);
    free(ys);
}

// --------------------------------------- RECTANGLE ---------------------------------------

void KitraDrawRect(KitraRect rect, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    SDL_Rect sdlRect = KitraToSDLRect(rect);
    SDL_SetRenderDrawColor(gKitraCtx.core.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(gKitraCtx.core.renderer, &sdlRect);
}

void KitraDrawRectOutline(KitraRect rect, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    SDL_Rect sdlRect = KitraToSDLRect(rect);
    SDL_SetRenderDrawColor(gKitraCtx.core.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(gKitraCtx.core.renderer, &sdlRect);
}

void KitraDrawRoundedRect(KitraRect rect, int radius, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    roundedBoxRGBA(gKitraCtx.core.renderer,
                   rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1,
                   radius, KITRA_COLOR_UNPACK(color));
}

void KitraDrawRoundedRectOutline(KitraRect rect, int radius, KitraColor color)
{
    if (!gKitraCtx.core.renderer)
        return;

    roundedRectangleRGBA(gKitraCtx.core.renderer,
                         rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1,
                         radius, KITRA_COLOR_UNPACK(color));
}