#ifndef CINDER_MATH_H_
#define CINDER_MATH_H_

#include <math.h>

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

// ======================================= POINT ================================================

typedef union CinderPoint
{
    int asArray[2];
    struct
    {
        int x, y;
    };
} CinderPoint;

// ======================================= POINT ================================================

typedef union CinderSize
{
    int asArray[2];
    struct
    {
        int w, h;
    };
} CinderSize;

// ======================================= RECTANGLE ================================================

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

#endif /* CINDER_MATH_H_ */