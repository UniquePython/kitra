#ifndef CINDER_MATH_H_
#define CINDER_MATH_H_

#include "cinder_types.h"

#include <math.h>
#include <stdbool.h>

/** @brief The mathematical constant π, accurate to 15 decimal places. */
#define CINDER_PI 3.141592653589793

/**
 * @brief Converts degrees to radians.
 * @param deg Angle in degrees.
 * @return Equivalent angle in radians.
 */
#define CINDER_DEG2RAD(deg) ((deg) * ((float)CINDER_PI / 180.0f))

/**
 * @brief Converts radians to degrees.
 * @param rad Angle in radians.
 * @return Equivalent angle in degrees.
 */
#define CINDER_RAD2DEG(rad) ((rad) * (180.0f / (float)CINDER_PI))

// ======================================= VECTORS ================================================

// --------------------------------------- 2D ---------------------------------------

/**
 * @brief Adds two integer vectors.
 * @param a First vector.
 * @param b Second vector.
 * @return Component-wise sum of @p a and @p b.
 */
static inline CinderVec2i CinderVec2iAdd(CinderVec2i a, CinderVec2i b)
{
    return (CinderVec2i){{a.x + b.x, a.y + b.y}};
}

/**
 * @brief Adds two float vectors.
 * @param a First vector.
 * @param b Second vector.
 * @return Component-wise sum of @p a and @p b.
 */
static inline CinderVec2f CinderVec2fAdd(CinderVec2f a, CinderVec2f b)
{
    return (CinderVec2f){{a.x + b.x, a.y + b.y}};
}

/**
 * @brief Subtracts one integer vector from another.
 * @param a First vector.
 * @param b Vector to subtract from @p a.
 * @return Component-wise difference of @p a and @p b.
 */
static inline CinderVec2i CinderVec2iSub(CinderVec2i a, CinderVec2i b)
{
    return (CinderVec2i){{a.x - b.x, a.y - b.y}};
}

/**
 * @brief Subtracts one float vector from another.
 * @param a First vector.
 * @param b Vector to subtract from @p a.
 * @return Component-wise difference of @p a and @p b.
 */
static inline CinderVec2f CinderVec2fSub(CinderVec2f a, CinderVec2f b)
{
    return (CinderVec2f){{a.x - b.x, a.y - b.y}};
}

/**
 * @brief Negates an integer vector.
 * @param v Vector to negate.
 * @return Vector with both components negated.
 */
static inline CinderVec2i CinderVec2iNegate(CinderVec2i v)
{
    return (CinderVec2i){{-v.x, -v.y}};
}

/**
 * @brief Negates a float vector.
 * @param v Vector to negate.
 * @return Vector with both components negated.
 */
static inline CinderVec2f CinderVec2fNegate(CinderVec2f v)
{
    return (CinderVec2f){{-v.x, -v.y}};
}

/**
 * @brief Scales an integer vector by a scalar.
 * @param v      Vector to scale.
 * @param factor Scalar multiplier.
 * @return Vector with both components multiplied by @p factor.
 */
static inline CinderVec2i CinderVec2iScale(CinderVec2i v, int factor)
{
    return (CinderVec2i){{v.x * factor, v.y * factor}};
}

/**
 * @brief Scales a float vector by a scalar.
 * @param v      Vector to scale.
 * @param factor Scalar multiplier.
 * @return Vector with both components multiplied by @p factor.
 */
static inline CinderVec2f CinderVec2fScale(CinderVec2f v, float factor)
{
    return (CinderVec2f){{v.x * factor, v.y * factor}};
}

/**
 * @brief Returns the squared length of an integer vector.
 *
 * Cheaper than CinderVec2iLength() as it avoids a square root.
 * Useful for distance comparisons where the exact length isn't needed.
 *
 * @param v Input vector.
 * @return x² + y².
 */
static inline int CinderVec2iLengthSquared(CinderVec2i v)
{
    return v.x * v.x + v.y * v.y;
}

/**
 * @brief Returns the squared length of a float vector.
 *
 * Cheaper than CinderVec2fLength() as it avoids a square root.
 * Useful for distance comparisons where the exact length isn't needed.
 *
 * @param v Input vector.
 * @return x² + y².
 */
static inline float CinderVec2fLengthSquared(CinderVec2f v)
{
    return v.x * v.x + v.y * v.y;
}

/**
 * @brief Returns the length of an integer vector.
 * @param v Input vector.
 * @return Euclidean length — √(x² + y²).
 */
static inline float CinderVec2iLength(CinderVec2i v)
{
    return sqrtf(CinderVec2iLengthSquared(v));
}

/**
 * @brief Returns the length of a float vector.
 * @param v Input vector.
 * @return Euclidean length — √(x² + y²).
 */
static inline float CinderVec2fLength(CinderVec2f v)
{
    return sqrtf(CinderVec2fLengthSquared(v));
}

/**
 * @brief Computes the dot product of two integer vectors.
 * @param a First vector.
 * @param b Second vector.
 * @return Scalar dot product — a.x*b.x + a.y*b.y.
 */
static inline int CinderVec2iDot(CinderVec2i a, CinderVec2i b)
{
    return a.x * b.x + a.y * b.y;
}

/**
 * @brief Computes the dot product of two float vectors.
 * @param a First vector.
 * @param b Second vector.
 * @return Scalar dot product — a.x*b.x + a.y*b.y.
 */
static inline float CinderVec2fDot(CinderVec2f a, CinderVec2f b)
{
    return a.x * b.x + a.y * b.y;
}

/**
 * @brief Normalizes a float vector to unit length.
 *
 * Returns a zero vector if the input length is zero, to avoid
 * division by zero.
 *
 * @param v Vector to normalize.
 * @return Unit vector in the same direction as @p v, or (0, 0) if @p v is zero.
 */
static inline CinderVec2f CinderVec2fNormalize(CinderVec2f v)
{
    float len = CinderVec2fLength(v);
    if (len == 0.0f)
        return (CinderVec2f){{0.0f, 0.0f}};
    return (CinderVec2f){{v.x / len, v.y / len}};
}

/**
 * @brief Linearly interpolates between two float vectors.
 * @param a Start vector (returned when @p t = 0).
 * @param b End vector (returned when @p t = 1).
 * @param t Interpolation factor, typically in [0, 1].
 * @return Interpolated vector between @p a and @p b.
 */
static inline CinderVec2f CinderVec2fLerp(CinderVec2f a, CinderVec2f b, float t)
{
    return (CinderVec2f){{a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t}};
}

/**
 * @brief Rotates a float vector by an angle.
 * @param v     Vector to rotate.
 * @param angle Rotation angle in radians, measured counter-clockwise.
 * @return Rotated vector.
 */
static inline CinderVec2f CinderVec2fRotate(CinderVec2f v, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    return (CinderVec2f){{v.x * c - v.y * s, v.x * s + v.y * c}};
}

/**
 * @brief Computes the 2D cross product of two integer vectors.
 *
 * Returns the Z component of the 3D cross product, which indicates
 * the signed area of the parallelogram formed by @p a and @p b.
 * Positive means @p b is counter-clockwise from @p a.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return a.x*b.y - a.y*b.x.
 */
static inline int CinderVec2iCross(CinderVec2i a, CinderVec2i b)
{
    return a.x * b.y - a.y * b.x;
}

/**
 * @brief Computes the 2D cross product of two float vectors.
 *
 * Returns the Z component of the 3D cross product, which indicates
 * the signed area of the parallelogram formed by @p a and @p b.
 * Positive means @p b is counter-clockwise from @p a.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return a.x*b.y - a.y*b.x.
 */
static inline float CinderVec2fCross(CinderVec2f a, CinderVec2f b)
{
    return a.x * b.y - a.y * b.x;
}

/**
 * @brief Returns the angle of a float vector in radians.
 * @param v Input vector.
 * @return Angle from the positive X axis in radians, in the range [-π, π].
 */
static inline float CinderVec2fAngle(CinderVec2f v)
{
    return atan2f(v.y, v.x);
}

// --------------------------------------- 3D ---------------------------------------

/**
 * @brief Adds two integer 3D vectors.
 * @param a First vector.
 * @param b Second vector.
 * @return Component-wise sum of @p a and @p b.
 */
static inline CinderVec3i CinderVec3iAdd(CinderVec3i a, CinderVec3i b)
{
    return (CinderVec3i){{a.x + b.x, a.y + b.y, a.z + b.z}};
}

/**
 * @brief Adds two float 3D vectors.
 * @param a First vector.
 * @param b Second vector.
 * @return Component-wise sum of @p a and @p b.
 */
static inline CinderVec3f CinderVec3fAdd(CinderVec3f a, CinderVec3f b)
{
    return (CinderVec3f){{a.x + b.x, a.y + b.y, a.z + b.z}};
}

/**
 * @brief Subtracts one integer 3D vector from another.
 * @param a First vector.
 * @param b Vector to subtract from @p a.
 * @return Component-wise difference of @p a and @p b.
 */
static inline CinderVec3i CinderVec3iSub(CinderVec3i a, CinderVec3i b)
{
    return (CinderVec3i){{a.x - b.x, a.y - b.y, a.z - b.z}};
}

/**
 * @brief Subtracts one float 3D vector from another.
 * @param a First vector.
 * @param b Vector to subtract from @p a.
 * @return Component-wise difference of @p a and @p b.
 */
static inline CinderVec3f CinderVec3fSub(CinderVec3f a, CinderVec3f b)
{
    return (CinderVec3f){{a.x - b.x, a.y - b.y, a.z - b.z}};
}

/**
 * @brief Negates an integer 3D vector.
 * @param v Vector to negate.
 * @return Vector with all components negated.
 */
static inline CinderVec3i CinderVec3iNegate(CinderVec3i v)
{
    return (CinderVec3i){{-v.x, -v.y, -v.z}};
}

/**
 * @brief Negates a float 3D vector.
 * @param v Vector to negate.
 * @return Vector with all components negated.
 */
static inline CinderVec3f CinderVec3fNegate(CinderVec3f v)
{
    return (CinderVec3f){{-v.x, -v.y, -v.z}};
}

/**
 * @brief Scales an integer 3D vector by a scalar.
 * @param v      Vector to scale.
 * @param factor Scalar multiplier.
 * @return Vector with all components multiplied by @p factor.
 */
static inline CinderVec3i CinderVec3iScale(CinderVec3i v, int factor)
{
    return (CinderVec3i){{v.x * factor, v.y * factor, v.z * factor}};
}

/**
 * @brief Scales a float 3D vector by a scalar.
 * @param v      Vector to scale.
 * @param factor Scalar multiplier.
 * @return Vector with all components multiplied by @p factor.
 */
static inline CinderVec3f CinderVec3fScale(CinderVec3f v, float factor)
{
    return (CinderVec3f){{v.x * factor, v.y * factor, v.z * factor}};
}

/**
 * @brief Returns the squared length of an integer 3D vector.
 *
 * Cheaper than CinderVec3iLength() as it avoids a square root.
 * Useful for distance comparisons where the exact length isn't needed.
 *
 * @param v Input vector.
 * @return x² + y² + z².
 */
static inline int CinderVec3iLengthSquared(CinderVec3i v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

/**
 * @brief Returns the squared length of a float 3D vector.
 *
 * Cheaper than CinderVec3fLength() as it avoids a square root.
 * Useful for distance comparisons where the exact length isn't needed.
 *
 * @param v Input vector.
 * @return x² + y² + z².
 */
static inline float CinderVec3fLengthSquared(CinderVec3f v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

/**
 * @brief Returns the length of an integer 3D vector.
 * @param v Input vector.
 * @return Euclidean length — √(x² + y² + z²).
 */
static inline float CinderVec3iLength(CinderVec3i v)
{
    return sqrtf(CinderVec3iLengthSquared(v));
}

/**
 * @brief Returns the length of a float 3D vector.
 * @param v Input vector.
 * @return Euclidean length — √(x² + y² + z²).
 */
static inline float CinderVec3fLength(CinderVec3f v)
{
    return sqrtf(CinderVec3fLengthSquared(v));
}

/**
 * @brief Computes the dot product of two integer 3D vectors.
 * @param a First vector.
 * @param b Second vector.
 * @return Scalar dot product — a.x*b.x + a.y*b.y + a.z*b.z.
 */
static inline int CinderVec3iDot(CinderVec3i a, CinderVec3i b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/**
 * @brief Computes the dot product of two float 3D vectors.
 * @param a First vector.
 * @param b Second vector.
 * @return Scalar dot product — a.x*b.x + a.y*b.y + a.z*b.z.
 */
static inline float CinderVec3fDot(CinderVec3f a, CinderVec3f b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/**
 * @brief Normalizes a float 3D vector to unit length.
 *
 * Returns a zero vector if the input length is zero, to avoid
 * division by zero.
 *
 * @param v Vector to normalize.
 * @return Unit vector in the same direction as @p v, or (0, 0, 0) if @p v is zero.
 */
static inline CinderVec3f CinderVec3fNormalize(CinderVec3f v)
{
    float len = CinderVec3fLength(v);
    if (len == 0.0f)
        return (CinderVec3f){{0.0f, 0.0f, 0.0f}};
    return (CinderVec3f){{v.x / len, v.y / len, v.z / len}};
}

// ======================================= COLLISION ================================================

/**
 * @brief Tests whether a point lies inside a rectangle.
 * @param p    Point to test.
 * @param rect Rectangle to test against.
 * @return True if @p p is inside @p rect, false otherwise.
 */
static inline bool CinderPointInRect(CinderPoint p, CinderRect rect)
{
    return p.x >= rect.x && p.x < rect.x + rect.w && p.y >= rect.y && p.y < rect.y + rect.h;
}

/**
 * @brief Tests whether a point lies inside a circle.
 * @param p      Point to test.
 * @param circle Circle to test against.
 * @return True if @p p is inside @p circle, false otherwise.
 */
static inline bool CinderPointInCircle(CinderPoint p, CinderCircle circle)
{
    int dx = p.x - circle.x;
    int dy = p.y - circle.y;
    return dx * dx + dy * dy < circle.radius * circle.radius;
}

/**
 * @brief Tests whether two rectangles overlap.
 * @param a First rectangle.
 * @param b Second rectangle.
 * @return True if @p a and @p b intersect, false otherwise.
 */
static inline bool CinderRectsOverlap(CinderRect a, CinderRect b)
{
    return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y;
}

/**
 * @brief Tests whether two circles overlap.
 * @param a First circle.
 * @param b Second circle.
 * @return True if @p a and @p b intersect, false otherwise.
 */
static inline bool CinderCirclesOverlap(CinderCircle a, CinderCircle b)
{
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    int radSum = a.radius + b.radius;
    return dx * dx + dy * dy < radSum * radSum;
}

/**
 * @brief Tests whether a rectangle and a circle overlap.
 *
 * Uses the nearest-point method — finds the closest point on the
 * rectangle to the circle center and checks if it falls within
 * the circle's radius.
 *
 * @param rect   Rectangle to test.
 * @param circle Circle to test against.
 * @return True if @p rect and @p circle intersect, false otherwise.
 */
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

/**
 * @brief Returns the area of a rectangle.
 * @param rect Input rectangle.
 * @return Area in pixels — w × h.
 */
static inline int CinderRectArea(CinderRect rect)
{
    return rect.w * rect.h;
}

/**
 * @brief Returns the perimeter of a rectangle.
 * @param rect Input rectangle.
 * @return Perimeter in pixels — 2 × (w + h).
 */
static inline int CinderRectPerimeter(CinderRect rect)
{
    return 2 * (rect.w + rect.h);
}

/**
 * @brief Returns the area of a circle.
 * @param circle Input circle.
 * @return Area in pixels — π × r².
 */
static inline float CinderCircleArea(CinderCircle circle)
{
    return (float)CINDER_PI * (float)circle.radius * (float)circle.radius;
}

/**
 * @brief Returns the circumference of a circle.
 * @param circle Input circle.
 * @return Circumference in pixels — 2 × π × r.
 */
static inline float CinderCirclePerimeter(CinderCircle circle)
{
    return 2.0f * (float)CINDER_PI * (float)circle.radius;
}

// ======================================= EASING ================================================

/**
 * @brief Linear easing — no acceleration or deceleration.
 * @param t Interpolation factor in [0, 1].
 * @return @p t unchanged.
 */
static inline float CinderEaseLinear(float t)
{
    return t;
}

/**
 * @brief Quadratic ease-in — accelerates from zero.
 * @param t Interpolation factor in [0, 1].
 * @return Eased value in [0, 1].
 */
static inline float CinderEaseInQuad(float t)
{
    return t * t;
}

/**
 * @brief Quadratic ease-out — decelerates to zero.
 * @param t Interpolation factor in [0, 1].
 * @return Eased value in [0, 1].
 */
static inline float CinderEaseOutQuad(float t)
{
    return t * (2.0f - t);
}

/**
 * @brief Quadratic ease-in-out — accelerates then decelerates.
 * @param t Interpolation factor in [0, 1].
 * @return Eased value in [0, 1].
 */
static inline float CinderEaseInOutQuad(float t)
{
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}

/**
 * @brief Sine ease-in — accelerates from zero using a sine curve.
 * @param t Interpolation factor in [0, 1].
 * @return Eased value in [0, 1].
 */
static inline float CinderEaseInSine(float t)
{
    return 1.0f - cosf(t * (float)CINDER_PI * 0.5f);
}

/**
 * @brief Sine ease-out — decelerates to zero using a sine curve.
 * @param t Interpolation factor in [0, 1].
 * @return Eased value in [0, 1].
 */
static inline float CinderEaseOutSine(float t)
{
    return sinf(t * (float)CINDER_PI * 0.5f);
}

/**
 * @brief Sine ease-in-out — accelerates then decelerates using a sine curve.
 * @param t Interpolation factor in [0, 1].
 * @return Eased value in [0, 1].
 */
static inline float CinderEaseInOutSine(float t)
{
    return 0.5f * (1.0f - cosf(t * (float)CINDER_PI));
}

/**
 * @brief Back ease-in — overshoots slightly before moving forward.
 *
 * Uses a back constant of 1.70158, producing a small negative dip
 * at the start before accelerating forward.
 *
 * @param t Interpolation factor in [0, 1].
 * @return Eased value, may dip slightly below 0 at the start.
 */
static inline float CinderEaseInBack(float t)
{
    const float c = 1.70158f;
    return t * t * ((c + 1.0f) * t - c);
}

/**
 * @brief Back ease-out — overshoots the target then settles back.
 *
 * Uses a back constant of 1.70158, producing a small overshoot
 * past 1 before settling at the final value.
 *
 * @param t Interpolation factor in [0, 1].
 * @return Eased value, may briefly exceed 1 near the end.
 */
static inline float CinderEaseOutBack(float t)
{
    const float c = 1.70158f;
    float u = t - 1.0f;
    return u * u * ((c + 1.0f) * u + c) + 1.0f;
}

#endif /* CINDER_MATH_H_ */