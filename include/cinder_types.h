#ifndef CINDER_TYPES_H_
#define CINDER_TYPES_H_

#include <stdint.h>

// ======================================= VECTORS ================================================

// --------------------------------------- 2D ---------------------------------------

typedef union CinderVec2i
{
    int axes[2];
    struct
    {
        int x, y;
    };
} CinderVec2i;

typedef union CinderVec2f
{
    float axes[2];
    struct
    {
        float x, y;
    };
} CinderVec2f;

// --------------------------------------- 3D ---------------------------------------

typedef union CinderVec3i
{
    int axes[3];
    struct
    {
        int x, y, z;
    };
} CinderVec3i;

typedef union CinderVec3f
{
    float axes[3];
    struct
    {
        float x, y, z;
    };
} CinderVec3f;

// ======================================= SCREEN TYPES ================================================

typedef union CinderPoint
{
    int asArray[2];
    struct
    {
        int x, y;
    };
} CinderPoint;

typedef union CinderSize
{
    int asArray[2];
    struct
    {
        int w, h;
    };
} CinderSize;

// ======================================= SHAPES ================================================

typedef union CinderRect
{
    struct
    {
        int x, y, w, h;
    };
    struct
    {
        CinderPoint pos;
        CinderSize size;
    };
} CinderRect;

typedef union CinderCircle
{
    struct
    {
        int x, y, r;
    };
    struct
    {
        CinderPoint center;
        int radius;
    };
} CinderCircle;

typedef union CinderEllipse
{
    struct
    {
        int x, y, rx, ry;
    };
    struct
    {
        CinderPoint center;
        CinderPoint radius;
    };
} CinderEllipse;

// ======================================= COLOR ================================================

typedef struct CinderColor
{
    uint8_t r, g, b, a;
} CinderColor;

#endif /* CINDER_TYPES_H_ */