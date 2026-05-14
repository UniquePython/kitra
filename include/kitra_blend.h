#ifndef KITRA_BLEND_H_
#define KITRA_BLEND_H_

#include "kitra_types.h"
#include "kitra_texture.h"
#include "kitra_status.h"

/**
 * @brief Blend modes controlling how a texture is composited onto the render target.
 *
 * Passed to blending functions to control how source and destination pixel
 * colours are combined during rendering.
 *
 * @see KitraSetTextureBlendMode
 */
typedef enum KitraBlendMode
{
    KITRA_BLEND_NONE,     /**< No blending — source pixels overwrite the destination. */
    KITRA_BLEND_ALPHA,    /**< Alpha blending — source is composited over destination using its alpha channel. */
    KITRA_BLEND_ADDITIVE, /**< Additive blending — source colour is added to the destination, brightening it. */
    KITRA_BLEND_MULTIPLY, /**< Multiply blending — source and destination colours are multiplied together, darkening the result. */

} KitraBlendMode;

/**
 * @brief Sets the blend mode used for primitive drawing operations.
 *
 * Controls how shapes and lines drawn with the renderer (rather than
 * textures) are composited onto the render target.
 *
 * @param mode  Blend mode to apply to subsequent draw calls.
 * @return      @p KITRA_STATUS_OK on success, or
 *              @p KITRA_STATUS_RENDERER_MISSING if the renderer is @p NULL.
 *
 * @see KitraSetTextureBlendMode, KitraBlendMode
 */
KitraStatus KitraSetBlendMode(KitraBlendMode mode);

/**
 * @brief Sets the blend mode of a texture.
 *
 * Controls how @p tex is composited onto the render target when drawn.
 * The mode is stored on the texture itself and applies whenever it is
 * rendered.
 *
 * @param tex   Texture to modify.
 * @param mode  Blend mode to apply to @p tex.
 * @return      @p KITRA_STATUS_OK on success, or
 *              @p KITRA_STATUS_TEXTURE_NULL if @p tex is @p NULL.
 *
 * @see KitraSetBlendMode, KitraBlendMode
 */
KitraStatus KitraSetTextureBlendMode(KitraTexture *tex, KitraBlendMode mode);

#endif /* KITRA_BLEND_H_ */