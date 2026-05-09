#ifndef CINDER_TYPES_H_
#define CINDER_TYPES_H_

#include <stdint.h>

// ======================================= VECTORS ================================================

// --------------------------------------- 2D ---------------------------------------

/**
 * @brief A 2D integer vector.
 *
 * Can be accessed either by name (x, y) or by index (axes[0], axes[1]).
 * Commonly used for pixel positions, mouse deltas, and scroll amounts.
 *
 * @see CinderVec2f
 */
typedef union CinderVec2i
{
    int axes[2]; /**< Component array — axes[0] is x, axes[1] is y. */
    struct
    {
        int x; /**< Horizontal component. */
        int y; /**< Vertical component. */
    };
} CinderVec2i;

/**
 * @brief A 2D float vector.
 *
 * Can be accessed either by name (x, y) or by index (axes[0], axes[1]).
 * Commonly used for directions, velocities, and continuous positions.
 *
 * @see CinderVec2i
 */
typedef union CinderVec2f
{
    float axes[2]; /**< Component array — axes[0] is x, axes[1] is y. */
    struct
    {
        float x; /**< Horizontal component. */
        float y; /**< Vertical component. */
    };
} CinderVec2f;

// --------------------------------------- 3D ---------------------------------------

/**
 * @brief A 3D integer vector.
 *
 * Can be accessed either by name (x, y, z) or by index (axes[0..2]).
 * Commonly used for 3D integer coordinates and color components.
 *
 * @see CinderVec3f
 */
typedef union CinderVec3i
{
    int axes[3]; /**< Component array — axes[0] is x, axes[1] is y, axes[2] is z. */
    struct
    {
        int x; /**< First component. */
        int y; /**< Second component. */
        int z; /**< Third component. */
    };
} CinderVec3i;

/**
 * @brief A 3D float vector.
 *
 * Can be accessed either by name (x, y, z) or by index (axes[0..2]).
 * Commonly used for 3D directions, velocities, and continuous coordinates.
 *
 * @see CinderVec3i
 */
typedef union CinderVec3f
{
    float axes[3]; /**< Component array — axes[0] is x, axes[1] is y, axes[2] is z. */
    struct
    {
        float x; /**< First component. */
        float y; /**< Second component. */
        float z; /**< Third component. */
    };
} CinderVec3f;

// ======================================= SCREEN TYPES ================================================

/**
 * @brief A 2D integer point representing a position.
 *
 * Can be accessed either by name (x, y) or by index (asArray[0], asArray[1]).
 * Commonly used for window coordinates, mouse positions, and draw targets.
 *
 * @see CinderSize, CinderRect
 */
typedef union CinderPoint
{
    int asArray[2]; /**< Component array — asArray[0] is x, asArray[1] is y. */
    struct
    {
        int x; /**< Horizontal position. */
        int y; /**< Vertical position. */
    };
} CinderPoint;

/**
 * @brief A 2D integer size representing dimensions.
 *
 * Can be accessed either by name (w, h) or by index (asArray[0], asArray[1]).
 * Commonly used for window dimensions, texture sizes, and bounding boxes.
 *
 * @see CinderPoint, CinderRect
 */
typedef union CinderSize
{
    int asArray[2]; /**< Component array — asArray[0] is w, asArray[1] is h. */
    struct
    {
        int w; /**< Width. */
        int h; /**< Height. */
    };
} CinderSize;

// ======================================= SHAPES ================================================

/**
 * @brief A 2D integer rectangle.
 *
 * Can be accessed as individual components (x, y, w, h) or as a
 * position and size pair (pos, size).
 *
 * @see CinderPoint, CinderSize, CinderCircle
 */
typedef union CinderRect
{
    struct
    {
        int x; /**< Horizontal position of the top-left corner. */
        int y; /**< Vertical position of the top-left corner. */
        int w; /**< Width. */
        int h; /**< Height. */
    };
    struct
    {
        CinderPoint pos; /**< Position of the top-left corner. */
        CinderSize size; /**< Dimensions of the rectangle. */
    };
} CinderRect;

/**
 * @brief A 2D integer circle.
 *
 * Can be accessed as individual components (x, y, r) or as a
 * center point and radius (center, radius).
 *
 * @see CinderPoint, CinderEllipse
 */
typedef union CinderCircle
{
    struct
    {
        int x; /**< Horizontal position of the center. */
        int y; /**< Vertical position of the center. */
        int r; /**< Radius. */
    };
    struct
    {
        CinderPoint center; /**< Center point of the circle. */
        int radius;         /**< Radius of the circle. */
    };
} CinderCircle;

/**
 * @brief A 2D integer ellipse.
 *
 * Can be accessed as individual components (x, y, rx, ry) or as a
 * center point and radius pair (center, radius).
 *
 * @see CinderPoint, CinderCircle
 */
typedef union CinderEllipse
{
    struct
    {
        int x;  /**< Horizontal position of the center. */
        int y;  /**< Vertical position of the center. */
        int rx; /**< Horizontal radius. */
        int ry; /**< Vertical radius. */
    };
    struct
    {
        CinderPoint center; /**< Center point of the ellipse. */
        CinderPoint radius; /**< Horizontal (x) and vertical (y) radii. */
    };
} CinderEllipse;

// ======================================= COLOR ================================================

/**
 * @brief An RGBA color with 8 bits per channel.
 *
 * Each component ranges from 0 to 255. The alpha channel controls
 * transparency — 0 is fully transparent, 255 is fully opaque.
 *
 * @see CINDER_BLACK, CINDER_WHITE, CinderColorLerp(), CinderColorFromU32()
 */
typedef struct CinderColor
{
    uint8_t r; /**< Red channel (0–255). */
    uint8_t g; /**< Green channel (0–255). */
    uint8_t b; /**< Blue channel (0–255). */
    uint8_t a; /**< Alpha channel (0–255). 0 = fully transparent, 255 = fully opaque. */
} CinderColor;

#endif /* CINDER_TYPES_H_ */