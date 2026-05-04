#ifndef CINDER_MATH_H_
#define CINDER_MATH_H_

#include "cinder_types.h"

#include <math.h>
#include <stdbool.h>

#define CINDER_PI 3.141592653589793

// ======================================= VECTORS ================================================

// --------------------------------------- 2D ---------------------------------------

static inline CinderVec2i CinderVec2iAdd(CinderVec2i a, CinderVec2i b)
{
    return (CinderVec2i){{a.x + b.x, a.y + b.y}};
}

static inline CinderVec2f CinderVec2fAdd(CinderVec2f a, CinderVec2f b)
{
    return (CinderVec2f){{a.x + b.x, a.y + b.y}};
}

static inline CinderVec2i CinderVec2iSub(CinderVec2i a, CinderVec2i b)
{
    return (CinderVec2i){{a.x - b.x, a.y - b.y}};
}

static inline CinderVec2f CinderVec2fSub(CinderVec2f a, CinderVec2f b)
{
    return (CinderVec2f){{a.x - b.x, a.y - b.y}};
}

static inline CinderVec2i CinderVec2iNegate(CinderVec2i v)
{
    return (CinderVec2i){{-v.x, -v.y}};
}

static inline CinderVec2f CinderVec2fNegate(CinderVec2f v)
{
    return (CinderVec2f){{-v.x, -v.y}};
}

static inline CinderVec2i CinderVec2iScale(CinderVec2i v, int factor)
{
    return (CinderVec2i){{v.x * factor, v.y * factor}};
}

static inline CinderVec2f CinderVec2fScale(CinderVec2f v, float factor)
{
    return (CinderVec2f){{v.x * factor, v.y * factor}};
}

static inline int CinderVec2iLengthSquared(CinderVec2i v)
{
    return v.x * v.x + v.y * v.y;
}

static inline float CinderVec2fLengthSquared(CinderVec2f v)
{
    return v.x * v.x + v.y * v.y;
}

static inline float CinderVec2iLength(CinderVec2i v)
{
    return sqrtf(CinderVec2iLengthSquared(v));
}

static inline float CinderVec2fLength(CinderVec2f v)
{
    return sqrtf(CinderVec2fLengthSquared(v));
}

static inline int CinderVec2iDot(CinderVec2i a, CinderVec2i b)
{
    return a.x * b.x + a.y * b.y;
}

static inline float CinderVec2fDot(CinderVec2f a, CinderVec2f b)
{
    return a.x * b.x + a.y * b.y;
}

static inline CinderVec2f CinderVec2fNormalize(CinderVec2f v)
{
    float len = CinderVec2fLength(v);
    if (len == 0.0f)
        return (CinderVec2f){{0.0f, 0.0f}};
    return (CinderVec2f){{v.x / len, v.y / len}};
}

// --------------------------------------- 3D ---------------------------------------

static inline CinderVec3i CinderVec3iAdd(CinderVec3i a, CinderVec3i b)
{
    return (CinderVec3i){{a.x + b.x, a.y + b.y, a.z + b.z}};
}

static inline CinderVec3f CinderVec3fAdd(CinderVec3f a, CinderVec3f b)
{
    return (CinderVec3f){{a.x + b.x, a.y + b.y, a.z + b.z}};
}

static inline CinderVec3i CinderVec3iSub(CinderVec3i a, CinderVec3i b)
{
    return (CinderVec3i){{a.x - b.x, a.y - b.y, a.z - b.z}};
}

static inline CinderVec3f CinderVec3fSub(CinderVec3f a, CinderVec3f b)
{
    return (CinderVec3f){{a.x - b.x, a.y - b.y, a.z - b.z}};
}

static inline CinderVec3i CinderVec3iNegate(CinderVec3i v)
{
    return (CinderVec3i){{-v.x, -v.y, -v.z}};
}

static inline CinderVec3f CinderVec3fNegate(CinderVec3f v)
{
    return (CinderVec3f){{-v.x, -v.y, -v.z}};
}

static inline CinderVec3i CinderVec3iScale(CinderVec3i v, int factor)
{
    return (CinderVec3i){{v.x * factor, v.y * factor, v.z * factor}};
}

static inline CinderVec3f CinderVec3fScale(CinderVec3f v, float factor)
{
    return (CinderVec3f){{v.x * factor, v.y * factor, v.z * factor}};
}

static inline int CinderVec3iLengthSquared(CinderVec3i v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline float CinderVec3fLengthSquared(CinderVec3f v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline float CinderVec3iLength(CinderVec3i v)
{
    return sqrtf(CinderVec3iLengthSquared(v));
}

static inline float CinderVec3fLength(CinderVec3f v)
{
    return sqrtf(CinderVec3fLengthSquared(v));
}

static inline int CinderVec3iDot(CinderVec3i a, CinderVec3i b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline float CinderVec3fDot(CinderVec3f a, CinderVec3f b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline CinderVec3f CinderVec3fNormalize(CinderVec3f v)
{
    float len = CinderVec3fLength(v);
    if (len == 0.0f)
        return (CinderVec3f){{0.0f, 0.0f, 0.0f}};
    return (CinderVec3f){{v.x / len, v.y / len, v.z / len}};
}

// ======================================= COLLISION ================================================

static inline bool CinderPointInRect(CinderPoint p, CinderRect rect)
{
    return p.x >= rect.x && p.x < rect.x + rect.w && p.y >= rect.y && p.y < rect.y + rect.h;
}

static inline bool CinderPointInCircle(CinderPoint p, CinderCircle circle)
{
    int dx = p.x - circle.x;
    int dy = p.y - circle.y;
    return dx * dx + dy * dy < circle.radius * circle.radius;
}

static inline bool CinderRectsOverlap(CinderRect a, CinderRect b)
{
    return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y;
}

static inline bool CinderCirclesOverlap(CinderCircle a, CinderCircle b)
{
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    int radSum = a.radius + b.radius;
    return dx * dx + dy * dy < radSum * radSum;
}

static inline bool CinderRectCircleOverlap(CinderRect rect, CinderCircle circle)
{
    int nearX = circle.x < rect.x ? rect.x : circle.x > rect.x + rect.w ? rect.x + rect.w
                                                                        : circle.x;
    int nearY = circle.y < rect.y ? rect.y : circle.y > rect.y + rect.h ? rect.y + rect.h
                                                                        : circle.y;
    int dx = circle.x - nearX;
    int dy = circle.y - nearY;
    return dx * dx + dy * dy < circle.radius * circle.radius;
}

// ======================================= GEOMETRY ================================================

static inline int CinderRectArea(CinderRect rect)
{
    return rect.w * rect.h;
}

static inline int CinderRectPerimeter(CinderRect rect)
{
    return 2 * (rect.w + rect.h);
}

static inline float CinderCircleArea(CinderCircle circle)
{
    return (float)CINDER_PI * (float)circle.radius * (float)circle.radius;
}

static inline float CinderCirclePerimeter(CinderCircle circle)
{
    return 2.0f * (float)CINDER_PI * (float)circle.radius;
}

// ======================================= EASING ================================================

static inline float CinderEaseLinear(float t)
{
    return t;
}

static inline float CinderEaseInQuad(float t)
{
    return t * t;
}

static inline float CinderEaseOutQuad(float t)
{
    return t * (2.0f - t);
}

static inline float CinderEaseInOutQuad(float t)
{
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}

static inline float CinderEaseInSine(float t)
{
    return 1.0f - cosf(t * (float)CINDER_PI * 0.5f);
}

static inline float CinderEaseOutSine(float t)
{
    return sinf(t * (float)CINDER_PI * 0.5f);
}

static inline float CinderEaseInOutSine(float t)
{
    return 0.5f * (1.0f - cosf(t * (float)CINDER_PI));
}

static inline float CinderEaseInBack(float t)
{
    const float c = 1.70158f;
    return t * t * ((c + 1.0f) * t - c);
}

static inline float CinderEaseOutBack(float t)
{
    const float c = 1.70158f;
    float u = t - 1.0f;
    return u * u * ((c + 1.0f) * u + c) + 1.0f;
}

#endif /* CINDER_MATH_H_ */