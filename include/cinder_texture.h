#ifndef CINDER_TEXTURE_H_
#define CINDER_TEXTURE_H_

#include "cinder_types.h"

typedef struct CinderTexture CinderTexture;

/**
 * @brief Loads an image file as a texture.
 *
 * Initializes the image subsystem automatically if it has not been
 * initialized yet, with support for PNG, JPG, WebP, and TIFF formats
 * (PNG and JPG are required; others are loaded opportunistically).
 * Allocates and returns a new @p CinderTexture uploaded to the GPU and
 * ready for rendering.
 *
 * @param path  Path to the image file to load.
 * @return      Pointer to a newly allocated @p CinderTexture on success, or
 *              @p NULL if the image subsystem could not be initialized, the
 *              renderer is not initialized, the file could not be loaded,
 *              texture creation fails, or memory allocation fails.
 *
 * @note The caller is responsible for freeing the returned texture with
 *       @p CinderDestroyTexture when it is no longer needed.
 *
 * @see CinderDestroyTexture, CinderDrawTexture, CinderSetTextureBlendMode
 */
CinderTexture *CinderLoadTexture(const char *path);

/**
 * @brief Destroys a texture and releases all associated resources.
 *
 * Destroys the underlying SDL texture and frees the @p CinderTexture
 * allocation, then sets the caller's pointer to @p NULL to prevent dangling
 * references. Safe to call with @p NULL or a pointer to @p NULL.
 *
 * @param tex  Address of the @p CinderTexture pointer to destroy.
 *
 * @see CinderLoadTexture
 */
void CinderDestroyTexture(CinderTexture **tex);

/**
 * @brief Returns the pixel dimensions of a texture.
 *
 * Retrieves the width and height stored on @p tex at load time, without
 * querying the GPU.
 *
 * @param tex  Texture to query.
 * @return     Dimensions of @p tex as a @p CinderSize, or a zero
 *             @p CinderSize if @p tex is @p NULL.
 *
 * @see CinderLoadTexture, CinderSize
 */
CinderSize CinderGetTextureSize(const CinderTexture *tex);

/**
 * @brief Saves a screenshot of the current render target to a PNG file.
 *
 * Reads the current contents of the render target and writes them to @p path
 * as a PNG image. The output dimensions match the current window size.
 *
 * @param path  Destination file path for the PNG screenshot.
 * @return      @p CINDER_STATUS_OK on success, or
 *              @p CINDER_STATUS_SCREENSHOT_FAILED if @p path is @p NULL,
 *              the renderer is not initialized, pixel readback fails, or
 *              the PNG could not be written.
 *
 * @see CinderGetWindowSize
 */
CinderStatus CinderScreenshot(const char *path);

/**
 * @brief Draws a texture with explicit source and destination rectangles.
 *
 * Copies a region of @p tex defined by @p src onto the render target at the
 * region defined by @p dst, scaling to fit if the rectangles differ in size.
 * Passing @p NULL for @p src uses the entire texture; passing @p NULL for
 * @p dst stretches the texture to fill the entire render target. Does nothing
 * if the renderer or @p tex is @p NULL.
 *
 * @param tex  Texture to draw.
 * @param src  Source region within @p tex, or @p NULL for the full texture.
 * @param dst  Destination region on the render target, or @p NULL to fill
 *             the entire render target.
 *
 * @see CinderDrawTexture, CinderDrawTextureP, CinderSetTextureBlendMode
 */
void CinderDrawTextureEx(CinderTexture *tex, const CinderRect *src, const CinderRect *dst);

/**
 * @brief Draws a texture at the given coordinates at its natural size.
 *
 * Convenience wrapper around @p CinderDrawTextureEx that draws the full
 * texture at (@p x, @p y) without scaling.
 *
 * @param tex  Texture to draw.
 * @param x    Horizontal position of the top-left corner, in pixels.
 * @param y    Vertical position of the top-left corner, in pixels.
 *
 * @see CinderDrawTextureP, CinderDrawTextureEx
 */
void CinderDrawTexture(CinderTexture *tex, int x, int y);

/**
 * @brief Draws a texture at a given point at its natural size.
 *
 * Convenience wrapper around @p CinderDrawTexture that accepts a
 * @p CinderPoint instead of separate x and y coordinates.
 *
 * @param tex  Texture to draw.
 * @param pos  Position of the top-left corner, in pixels.
 *
 * @see CinderDrawTexture, CinderDrawTextureEx
 */
void CinderDrawTextureP(CinderTexture *tex, CinderPoint pos);

struct SDL_Texture;

/**
 * @brief Returns the underlying SDL texture handle.
 *
 * Provides access to the raw @p SDL_Texture pointer for interoperability
 * with SDL APIs not wrapped by Cinder. Ownership remains with @p tex —
 * do not destroy the returned pointer directly.
 *
 * @param tex  Texture to retrieve the handle from.
 * @return     The underlying @p SDL_Texture pointer.
 *
 * @see CinderSetTextureBlendMode
 */
struct SDL_Texture *CinderTextureGetSDL(CinderTexture *tex);

#endif /* CINDER_TEXTURE_H_ */