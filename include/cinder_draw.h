#ifndef CINDER_DRAW_H_
#define CINDER_DRAW_H_

#include "cinder_types.h"

void CinderClearBackground(CinderColor color);

// --------------------------------------- POINT ---------------------------------------

void CinderDrawPoint(int x, int y, CinderColor color);
void CinderDrawPointP(CinderPoint pos, CinderColor color);

// --------------------------------------- LINE ---------------------------------------

void CinderDrawLine(int x1, int y1, int x2, int y2, CinderColor color);
void CinderDrawLineP(CinderPoint start, CinderPoint end, CinderColor color);

void CinderDrawLineThick(int x1, int y1, int x2, int y2, int width, CinderColor color);
void CinderDrawLineThickP(CinderPoint start, CinderPoint end, int width, CinderColor color);

// --------------------------------------- CIRCLE ---------------------------------------

void CinderDrawCircle(int x, int y, int radius, CinderColor color);
void CinderDrawCircleC(CinderCircle circle, CinderColor color);

void CinderDrawCircleOutline(int x, int y, int radius, CinderColor color);
void CinderDrawCircleOutlineC(CinderCircle circle, CinderColor color);

// --------------------------------------- ELLIPSE ---------------------------------------

void CinderDrawEllipse(int x, int y, int rx, int ry, CinderColor color);
void CinderDrawEllipseP(CinderPoint pos, CinderPoint radius, CinderColor color);
void CinderDrawEllipseE(CinderEllipse ellipse, CinderColor color);

void CinderDrawEllipseOutline(int x, int y, int rx, int ry, CinderColor color);
void CinderDrawEllipseOutlineP(CinderPoint pos, CinderPoint radius, CinderColor color);
void CinderDrawEllipseOutlineE(CinderEllipse ellipse, CinderColor color);

// --------------------------------------- TRIANGLE ---------------------------------------

void CinderDrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, CinderColor color);
void CinderDrawTriangleP(CinderPoint p1, CinderPoint p2, CinderPoint p3, CinderColor color);

void CinderDrawTriangleOutline(int x1, int y1, int x2, int y2, int x3, int y3, CinderColor color);
void CinderDrawTriangleOutlineP(CinderPoint p1, CinderPoint p2, CinderPoint p3, CinderColor color);

// --------------------------------------- RECTANGLE ---------------------------------------

void CinderDrawRect(CinderRect rect, CinderColor color);
void CinderDrawRectOutline(CinderRect rect, CinderColor color);

void CinderDrawRoundedRect(CinderRect rect, int radius, CinderColor color);
void CinderDrawRoundedRectOutline(CinderRect rect, int radius, CinderColor color);

#endif /* CINDER_DRAW_H_ */