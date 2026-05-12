#ifndef CINDER_BLEND_H_
#define CINDER_BLEND_H_

#include "cinder_types.h"
#include "cinder_texture.h"
#include "cinder_status.h"

/**
 * @brief Blend modes controlling how a texture is composited onto the render target.
 *
 * Passed to blending functions to control how source and destination pixel
 * colours are combined during rendering.
 *
 * @see CinderSetTextureBlendMode
 */
typedef enum CinderBlendMode
{
    CINDER_BLEND_NONE,     /**< No blending — source pixels overwrite the destination. */
    CINDER_BLEND_ALPHA,    /**< Alpha blending — source is composited over destination using its alpha channel. */
    CINDER_BLEND_ADDITIVE, /**< Additive blending — source colour is added to the destination, brightening it. */
    CINDER_BLEND_MULTIPLY, /**< Multiply blending — source and destination colours are multiplied together, darkening the result. */

} CinderBlendMode;

/**
 * @brief Sets the blend mode used for primitive drawing operations.
 *
 * Controls how shapes and lines drawn with the renderer (rather than
 * textures) are composited onto the render target.
 *
 * @param mode  Blend mode to apply to subsequent draw calls.
 * @return      @p CINDER_STATUS_OK on success, or
 *              @p CINDER_STATUS_RENDERER_MISSING if the renderer is @p NULL.
 *
 * @see CinderSetTextureBlendMode, CinderBlendMode
 */
CinderStatus CinderSetBlendMode(CinderBlendMode mode);

/**
 * @brief Sets the blend mode of a texture.
 *
 * Controls how @p tex is composited onto the render target when drawn.
 * The mode is stored on the texture itself and applies whenever it is
 * rendered.
 *
 * @param tex   Texture to modify.
 * @param mode  Blend mode to apply to @p tex.
 * @return      @p CINDER_STATUS_OK on success, or
 *              @p CINDER_STATUS_TEXTURE_NULL if @p tex is @p NULL.
 *
 * @see CinderSetBlendMode, CinderBlendMode
 */
CinderStatus CinderSetTextureBlendMode(CinderTexture *tex, CinderBlendMode mode);

#endif /* CINDER_BLEND_H_ */