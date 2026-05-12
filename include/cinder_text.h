#ifndef CINDER_TEXT_H_
#define CINDER_TEXT_H_

#include "cinder_types.h"

typedef struct CinderFont CinderFont;
typedef struct CinderText CinderText;

/**
 * @brief Loads a TrueType font from a file at the given point size.
 *
 * Initializes the TTF subsystem automatically if it has not been initialized
 * yet. Allocates and returns a new @p CinderFont ready for use in text
 * rendering calls.
 *
 * @param path  Path to the TrueType (@p .ttf) font file to load.
 * @param size  Point size to render the font at. Must be greater than zero.
 * @return      Pointer to a newly allocated @p CinderFont on success, or
 *              @p NULL if @p path is @p NULL, @p size is invalid, the TTF
 *              subsystem could not be initialized, the file could not be
 *              loaded, or memory allocation failed.
 *
 * @note The caller is responsible for freeing the returned font with
 *       @p CinderDestroyFont when it is no longer needed.
 *
 * @see CinderDestroyFont, CinderDrawText
 */
CinderFont *CinderLoadFont(const char *path, int size);

/**
 * @brief Destroys a font and releases all associated resources.
 *
 * Closes the underlying @p TTF_Font handle and frees the @p CinderFont
 * allocation, then sets the caller's pointer to @p NULL to prevent dangling
 * references. Safe to call with @p NULL or a pointer to @p NULL.
 *
 * @param font  Address of the @p CinderFont pointer to destroy.
 *
 * @see CinderLoadFont
 */
void CinderDestroyFont(CinderFont **font);

/**
 * @brief Draws a UTF-8 string to the render target at the given coordinates.
 *
 * Rasterizes @p text using @p font and blits it to the render target at
 * (@p x, @p y), measured from the top-left corner of the rendered string.
 * A temporary texture is created and destroyed internally each call; for
 * frequently drawn text consider caching the result as a @p CinderTexture.
 * Does nothing if the renderer, @p font, or @p text is @p NULL.
 *
 * @param font   Font to render with.
 * @param text   UTF-8 string to render.
 * @param x      Horizontal position of the top-left corner of the text, in pixels.
 * @param y      Vertical position of the top-left corner of the text, in pixels.
 * @param color  RGBA colour to render the text in.
 *
 * @see CinderDrawTextP, CinderLoadFont, CinderColor
 */
void CinderDrawText(CinderFont *font, const char *text, int x, int y, CinderColor color);

/**
 * @brief Draws a UTF-8 string to the render target at a given point.
 *
 * Convenience wrapper around @p CinderDrawText that accepts a @p CinderPoint
 * instead of separate x and y coordinates.
 *
 * @param font   Font to render with.
 * @param text   UTF-8 string to render.
 * @param pos    Position of the top-left corner of the text, in pixels.
 * @param color  RGBA colour to render the text in.
 *
 * @see CinderDrawText, CinderLoadFont, CinderColor
 */
void CinderDrawTextP(CinderFont *font, const char *text, CinderPoint pos, CinderColor color);

/**
 * @brief Draws a formatted UTF-8 string to the render target at the given coordinates.
 *
 * Constructs a string from @p fmt and the supplied arguments using
 * @p printf-style formatting, then renders it via @p CinderDrawText.
 * The formatted string is allocated internally and freed before returning.
 *
 * @param font   Font to render with.
 * @param x      Horizontal position of the top-left corner of the text, in pixels.
 * @param y      Vertical position of the top-left corner of the text, in pixels.
 * @param color  RGBA colour to render the text in.
 * @param fmt    @p printf-style format string.
 * @param ...    Arguments for @p fmt.
 *
 * @see CinderDrawTextFP, CinderDrawText, CinderLoadFont, CinderColor
 */
void CinderDrawTextF(CinderFont *font, int x, int y, CinderColor color, const char *fmt, ...);

/**
 * @brief Draws a formatted UTF-8 string to the render target at a given point.
 *
 * Convenience wrapper around @p CinderDrawTextF that accepts a @p CinderPoint
 * instead of separate x and y coordinates.
 *
 * @param font   Font to render with.
 * @param pos    Position of the top-left corner of the text, in pixels.
 * @param color  RGBA colour to render the text in.
 * @param fmt    @p printf-style format string.
 * @param ...    Arguments for @p fmt.
 *
 * @see CinderDrawTextF, CinderDrawText, CinderLoadFont, CinderColor
 */
void CinderDrawTextFP(CinderFont *font, CinderPoint pos, CinderColor color, const char *fmt, ...);

/**
 * @brief Creates a pre-rasterized text object from a string.
 *
 * Rasterizes @p text into a @p CinderTexture-backed @p CinderText object
 * that can be drawn repeatedly without re-rasterizing each frame. Prefer
 * this over @p CinderDrawText for strings that are drawn frequently or
 * whose content changes rarely.
 *
 * @param font   Font to rasterize with.
 * @param text   UTF-8 string to rasterize.
 * @param color  RGBA colour to rasterize the text in.
 * @return       Pointer to a newly allocated @p CinderText on success, or
 *               @p NULL if the renderer, @p font, or @p text is @p NULL,
 *               rasterization fails, texture creation fails, or memory
 *               allocation fails.
 *
 * @note The caller is responsible for freeing the returned object with
 *       @p CinderDestroyText when it is no longer needed.
 *
 * @see CinderDestroyText, CinderDrawText, CinderLoadFont
 */
CinderText *CinderCreateText(CinderFont *font, const char *text, CinderColor color);

/**
 * @brief Draws a pre-rasterized text object at the given coordinates.
 *
 * Blits the cached texture held by @p t to the render target at (@p x, @p y),
 * measured from the top-left corner of the text. Does nothing if the
 * renderer, @p t, or its internal handle is @p NULL.
 *
 * @param t  Pre-rasterized text object to draw.
 * @param x  Horizontal position of the top-left corner of the text, in pixels.
 * @param y  Vertical position of the top-left corner of the text, in pixels.
 *
 * @see CinderDrawCachedTextP, CinderCreateText, CinderDestroyCachedText
 */
void CinderDrawCachedText(CinderText *t, int x, int y);

/**
 * @brief Draws a pre-rasterized text object at a given point.
 *
 * Convenience wrapper around @p CinderDrawCachedText that accepts a
 * @p CinderPoint instead of separate x and y coordinates.
 *
 * @param t    Pre-rasterized text object to draw.
 * @param pos  Position of the top-left corner of the text, in pixels.
 *
 * @see CinderDrawCachedText, CinderCreateText, CinderDestroyCachedText
 */
void CinderDrawCachedTextP(CinderText *t, CinderPoint pos);

/**
 * @brief Destroys a pre-rasterized text object and releases all associated resources.
 *
 * Destroys the underlying SDL texture and frees the @p CinderText allocation,
 * then sets the caller's pointer to @p NULL to prevent dangling references.
 * Safe to call with @p NULL or a pointer to @p NULL.
 *
 * @param t  Address of the @p CinderText pointer to destroy.
 *
 * @see CinderCreateText, CinderDrawCachedText
 */
void CinderDestroyCachedText(CinderText **t);

/**
 * @brief Measures the pixel dimensions of a rendered UTF-8 string.
 *
 * Returns the bounding box that @p text would occupy if rendered with
 * @p font, without actually drawing anything. Useful for alignment,
 * layout, and clipping calculations.
 *
 * @param font  Font to measure with.
 * @param text  UTF-8 string to measure.
 * @return      Pixel dimensions of the rendered string as a @p CinderSize,
 *              or a zero @p CinderSize if @p font or @p text is @p NULL,
 *              or if measurement fails.
 *
 * @see CinderMeasureTextF, CinderDrawText, CinderLoadFont
 */
CinderSize CinderMeasureText(CinderFont *font, const char *text);

/**
 * @brief Measures the pixel dimensions of a formatted UTF-8 string.
 *
 * Constructs a string from @p fmt and the supplied arguments using
 * @p printf-style formatting, then measures it via @p CinderMeasureText.
 * The formatted string is allocated internally and freed before returning.
 *
 * @param font  Font to measure with.
 * @param fmt   @p printf-style format string.
 * @param ...   Arguments for @p fmt.
 * @return      Pixel dimensions of the rendered string as a @p CinderSize,
 *              or a zero @p CinderSize if formatting fails or memory
 *              allocation fails.
 *
 * @see CinderMeasureText, CinderDrawTextF, CinderLoadFont
 */
CinderSize CinderMeasureTextF(CinderFont *font, const char *fmt, ...);

#endif /* CINDER_TEXT_H_ */