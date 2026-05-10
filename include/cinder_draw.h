#ifndef CINDER_DRAW_H_
#define CINDER_DRAW_H_

#include "cinder_types.h"

/**
 * @brief Clears the entire screen with the given color.
 *
 * Typically called at the start of each frame before any drawing.
 * Logs a warning and returns early if the renderer is NULL.
 *
 * @param color Fill color.
 *
 * @see CinderBeginFrame()
 */
void CinderClearBackground(CinderColor color);

// --------------------------------------- POINT ---------------------------------------

/**
 * @brief Draws a single pixel at (x, y).
 *
 * @param x     Horizontal position.
 * @param y     Vertical position.
 * @param color Pixel color.
 *
 * @see CinderDrawPointP()
 */
void CinderDrawPoint(int x, int y, CinderColor color);

/**
 * @brief Draws a single pixel at the given point.
 *
 * Convenience wrapper around CinderDrawPoint().
 *
 * @param pos   Position.
 * @param color Pixel color.
 *
 * @see CinderDrawPoint()
 */
void CinderDrawPointP(CinderPoint pos, CinderColor color);

// --------------------------------------- LINE ---------------------------------------

/**
 * @brief Draws a line between two points.
 *
 * @param x1    X coordinate of the start point.
 * @param y1    Y coordinate of the start point.
 * @param x2    X coordinate of the end point.
 * @param y2    Y coordinate of the end point.
 * @param color Line color.
 *
 * @see CinderDrawLineP(), CinderDrawLineThick()
 */
void CinderDrawLine(int x1, int y1, int x2, int y2, CinderColor color);

/**
 * @brief Draws a line between two points.
 *
 * Convenience wrapper around CinderDrawLine().
 *
 * @param start Start point.
 * @param end   End point.
 * @param color Line color.
 *
 * @see CinderDrawLine()
 */
void CinderDrawLineP(CinderPoint start, CinderPoint end, CinderColor color);

/**
 * @brief Draws a thick line between two points.
 *
 * @param x1    X coordinate of the start point.
 * @param y1    Y coordinate of the start point.
 * @param x2    X coordinate of the end point.
 * @param y2    Y coordinate of the end point.
 * @param width Line width in pixels.
 * @param color Line color.
 *
 * @see CinderDrawLineThickP(), CinderDrawLine()
 */
void CinderDrawLineThick(int x1, int y1, int x2, int y2, int width, CinderColor color);

/**
 * @brief Draws a thick line between two points.
 *
 * Convenience wrapper around CinderDrawLineThick().
 *
 * @param start Start point.
 * @param end   End point.
 * @param width Line width in pixels.
 * @param color Line color.
 *
 * @see CinderDrawLineThick()
 */
void CinderDrawLineThickP(CinderPoint start, CinderPoint end, int width, CinderColor color);

// --------------------------------------- CIRCLE ---------------------------------------

/**
 * @brief Draws a filled circle.
 *
 * @param x      X coordinate of the center.
 * @param y      Y coordinate of the center.
 * @param radius Radius in pixels.
 * @param color  Fill color.
 *
 * @see CinderDrawCircleC(), CinderDrawCircleOutline()
 */
void CinderDrawCircle(int x, int y, int radius, CinderColor color);

/**
 * @brief Draws a filled circle.
 *
 * Convenience wrapper around CinderDrawCircle().
 *
 * @param circle Circle to draw.
 * @param color  Fill color.
 *
 * @see CinderDrawCircle()
 */
void CinderDrawCircleC(CinderCircle circle, CinderColor color);

/**
 * @brief Draws a circle outline.
 *
 * @param x      X coordinate of the center.
 * @param y      Y coordinate of the center.
 * @param radius Radius in pixels.
 * @param color  Outline color.
 *
 * @see CinderDrawCircleOutlineC(), CinderDrawCircle()
 */
void CinderDrawCircleOutline(int x, int y, int radius, CinderColor color);

/**
 * @brief Draws a circle outline.
 *
 * Convenience wrapper around CinderDrawCircleOutline().
 *
 * @param circle Circle to draw.
 * @param color  Outline color.
 *
 * @see CinderDrawCircleOutline()
 */
void CinderDrawCircleOutlineC(CinderCircle circle, CinderColor color);

// --------------------------------------- ELLIPSE ---------------------------------------

/**
 * @brief Draws a filled ellipse.
 *
 * @param x     X coordinate of the center.
 * @param y     Y coordinate of the center.
 * @param rx    Horizontal radius in pixels.
 * @param ry    Vertical radius in pixels.
 * @param color Fill color.
 *
 * @see CinderDrawEllipseP(), CinderDrawEllipseE(), CinderDrawEllipseOutline()
 */
void CinderDrawEllipse(int x, int y, int rx, int ry, CinderColor color);

/**
 * @brief Draws a filled ellipse using a center point and radius.
 *
 * Convenience wrapper around CinderDrawEllipse().
 *
 * @param pos    Center point.
 * @param radius Horizontal (x) and vertical (y) radii.
 * @param color  Fill color.
 *
 * @see CinderDrawEllipse()
 */
void CinderDrawEllipseP(CinderPoint pos, CinderPoint radius, CinderColor color);

/**
 * @brief Draws a filled ellipse.
 *
 * Convenience wrapper around CinderDrawEllipse().
 *
 * @param ellipse Ellipse to draw.
 * @param color   Fill color.
 *
 * @see CinderDrawEllipse()
 */
void CinderDrawEllipseE(CinderEllipse ellipse, CinderColor color);

/**
 * @brief Draws an ellipse outline.
 *
 * @param x     X coordinate of the center.
 * @param y     Y coordinate of the center.
 * @param rx    Horizontal radius in pixels.
 * @param ry    Vertical radius in pixels.
 * @param color Outline color.
 *
 * @see CinderDrawEllipseOutlineP(), CinderDrawEllipseOutlineE(), CinderDrawEllipse()
 */
void CinderDrawEllipseOutline(int x, int y, int rx, int ry, CinderColor color);

/**
 * @brief Draws an ellipse outline using a center point and radius.
 *
 * Convenience wrapper around CinderDrawEllipseOutline().
 *
 * @param pos    Center point.
 * @param radius Horizontal (x) and vertical (y) radii.
 * @param color  Outline color.
 *
 * @see CinderDrawEllipseOutline()
 */
void CinderDrawEllipseOutlineP(CinderPoint pos, CinderPoint radius, CinderColor color);

/**
 * @brief Draws an ellipse outline.
 *
 * Convenience wrapper around CinderDrawEllipseOutline().
 *
 * @param ellipse Ellipse to draw.
 * @param color   Outline color.
 *
 * @see CinderDrawEllipseOutline()
 */
void CinderDrawEllipseOutlineE(CinderEllipse ellipse, CinderColor color);

// --------------------------------------- TRIANGLE ---------------------------------------

/**
 * @brief Draws a filled triangle.
 *
 * @param x1    X coordinate of the first vertex.
 * @param y1    Y coordinate of the first vertex.
 * @param x2    X coordinate of the second vertex.
 * @param y2    Y coordinate of the second vertex.
 * @param x3    X coordinate of the third vertex.
 * @param y3    Y coordinate of the third vertex.
 * @param color Fill color.
 *
 * @see CinderDrawTriangleP(), CinderDrawTriangleOutline()
 */
void CinderDrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, CinderColor color);

/**
 * @brief Draws a filled triangle.
 *
 * Convenience wrapper around CinderDrawTriangle().
 *
 * @param p1    First vertex.
 * @param p2    Second vertex.
 * @param p3    Third vertex.
 * @param color Fill color.
 *
 * @see CinderDrawTriangle()
 */
void CinderDrawTriangleP(CinderPoint p1, CinderPoint p2, CinderPoint p3, CinderColor color);

/**
 * @brief Draws a triangle outline.
 *
 * @param x1    X coordinate of the first vertex.
 * @param y1    Y coordinate of the first vertex.
 * @param x2    X coordinate of the second vertex.
 * @param y2    Y coordinate of the second vertex.
 * @param x3    X coordinate of the third vertex.
 * @param y3    Y coordinate of the third vertex.
 * @param color Outline color.
 *
 * @see CinderDrawTriangleOutlineP(), CinderDrawTriangle()
 */
void CinderDrawTriangleOutline(int x1, int y1, int x2, int y2, int x3, int y3, CinderColor color);

/**
 * @brief Draws a triangle outline.
 *
 * Convenience wrapper around CinderDrawTriangleOutline().
 *
 * @param p1    First vertex.
 * @param p2    Second vertex.
 * @param p3    Third vertex.
 * @param color Outline color.
 *
 * @see CinderDrawTriangleOutline()
 */
void CinderDrawTriangleOutlineP(CinderPoint p1, CinderPoint p2, CinderPoint p3, CinderColor color);

// --------------------------------------- POLYGON ---------------------------------------

/**
 * @brief Draws a filled polygon.
 *
 * @p points must contain at least 3 points. Silently returns if the
 * renderer is NULL, @p points is NULL, or @p count is less than 3.
 *
 * @param points Array of vertices defining the polygon.
 * @param count  Number of vertices. Must be at least 3.
 * @param color  Fill color.
 *
 * @see CinderDrawPolygonOutline()
 */
void CinderDrawPolygon(const CinderPoint *points, int count, CinderColor color);

/**
 * @brief Draws a polygon outline.
 *
 * @p points must contain at least 3 points. Silently returns if the
 * renderer is NULL, @p points is NULL, or @p count is less than 3.
 *
 * @param points Array of vertices defining the polygon.
 * @param count  Number of vertices. Must be at least 3.
 * @param color  Outline color.
 *
 * @see CinderDrawPolygon()
 */
void CinderDrawPolygonOutline(const CinderPoint *points, int count, CinderColor color);

// --------------------------------------- RECTANGLE ---------------------------------------

/**
 * @brief Draws a filled rectangle.
 *
 * @param rect  Rectangle to draw.
 * @param color Fill color.
 *
 * @see CinderDrawRectOutline(), CinderDrawRoundedRect()
 */
void CinderDrawRect(CinderRect rect, CinderColor color);

/**
 * @brief Draws a rectangle outline.
 *
 * @param rect  Rectangle to draw.
 * @param color Outline color.
 *
 * @see CinderDrawRect(), CinderDrawRoundedRectOutline()
 */
void CinderDrawRectOutline(CinderRect rect, CinderColor color);

/**
 * @brief Draws a filled rectangle with rounded corners.
 *
 * @param rect   Rectangle to draw.
 * @param radius Corner radius in pixels.
 * @param color  Fill color.
 *
 * @see CinderDrawRoundedRectOutline(), CinderDrawRect()
 */
void CinderDrawRoundedRect(CinderRect rect, int radius, CinderColor color);

/**
 * @brief Draws a rounded rectangle outline.
 *
 * @param rect   Rectangle to draw.
 * @param radius Corner radius in pixels.
 * @param color  Outline color.
 *
 * @see CinderDrawRoundedRect(), CinderDrawRectOutline()
 */
void CinderDrawRoundedRectOutline(CinderRect rect, int radius, CinderColor color);

#endif /* CINDER_DRAW_H_ */