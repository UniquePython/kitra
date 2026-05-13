#ifndef CINDER_WINDOW_H_
#define CINDER_WINDOW_H_

#include "cinder_types.h"
#include "cinder_status.h"

#include <stdbool.h>

/**
 * @brief Flags controlling window creation behaviour.
 *
 * Bitmask values that can be combined with bitwise OR and passed to
 * @p CinderWindowDesc to configure the initial window state.
 *
 * @see CinderCreateWindow, CinderWindowDesc
 */
typedef enum
{
    CINDER_WINDOW_NONE = 0,             /**< No special window behaviour. */
    CINDER_WINDOW_SHOWN = 1u << 0,      /**< Window is visible on creation. */
    CINDER_WINDOW_RESIZABLE = 1u << 1,  /**< Window can be resized by the user. */
    CINDER_WINDOW_FULLSCREEN = 1u << 2, /**< Window starts in fullscreen mode. */
    CINDER_WINDOW_BORDERLESS = 1u << 3, /**< Window is created without a title bar or border. */
    CINDER_WINDOW_HIDDEN = 1u << 4,     /**< Window is hidden on creation. */

} CinderWindowFlags;

/**
 * @brief Descriptor for window creation parameters.
 *
 * Passed to @p CinderCreateWindow to configure the window. Position is specified
 * either via @p pos or by centering on one or both axes with @p centerX
 * and @p centerY; if an axis is centered, the corresponding component of
 * @p pos is ignored.
 *
 * @see CinderCreateWindow, CinderWindowFlags
 */
typedef struct CinderWindowDesc
{
    const char *title; /**< Window title shown in the title bar. */
    CinderSize size;   /**< Width and height of the window in pixels. */
    CinderPoint pos;   /**< Initial window position in screen coordinates. Ignored on axes where @p centerX or @p centerY is @p true. */

    bool centerX; /**< If @p true, the window is centered horizontally on the display. */
    bool centerY; /**< If @p true, the window is centered vertically on the display. */

    CinderWindowFlags flags; /**< Bitwise OR of @p CinderWindowFlags values controlling window behaviour. */

} CinderWindowDesc;

/**
 * @brief Returns a @p CinderWindowDesc populated with sensible defaults.
 *
 * The returned descriptor creates a 900×600 window titled "Cinder",
 * centered on the display, and initially visible and resizable.
 * Individual fields can be overridden after calling this function before
 * passing the descriptor to @p CinderCreateWindow.
 *
 * @return  A @p CinderWindowDesc with default values.
 *
 * @see CinderWindowDesc, CinderCreateWindow
 */
CinderWindowDesc CinderDefaultWindowDesc(void);

/**
 * @brief Creates the application window and hardware-accelerated renderer.
 *
 * Constructs an SDL window and renderer from @p winDesc, storing both in the
 * global engine context. On success the engine's main loop flag is set to
 * running. If renderer creation fails, the window is destroyed and the context
 * is cleaned up before returning.
 *
 * @param winDesc  Descriptor specifying the window's title, size, position,
 *                 and flags.
 * @return         @p CINDER_STATUS_OK on success, or one of:
 *                 - @p CINDER_STATUS_WINDOW_CREATE_FAILED if the window could
 *                   not be created.
 *                 - @p CINDER_STATUS_RENDERER_CREATE_FAILED if the
 *                   hardware-accelerated renderer could not be created.
 *
 * @see CinderDestroyWindow, CinderWindowDesc, CinderDefaultWindowDesc
 */
CinderStatus CinderCreateWindow(CinderWindowDesc winDesc);

/**
 * @brief Destroys the application window and renderer.
 *
 * Releases the renderer and window held in the global engine context, in that
 * order, and sets both pointers to @p NULL. Safe to call when either or both
 * have already been destroyed.
 *
 * @see CinderCreateWindow
 */
void CinderDestroyWindow(void);

/**
 * @brief Returns the current size of the application window.
 *
 * Queries the window for its current dimensions, reflecting any resizes
 * that have occurred since creation.
 *
 * @return  Current window dimensions as a @p CinderSize, or a zero
 *          @p CinderSize if the window is @p NULL.
 *
 * @see CinderIsWindowResized, CinderGetWindowDesc
 */
CinderSize CinderGetWindowSize(void);

/**
 * @brief Returns whether the window was resized this frame.
 *
 * @return  @p true if a resize event was received this frame,
 *          @p false otherwise.
 *
 * @see CinderGetWindowSize
 */
bool CinderIsWindowResized(void);

/**
 * @brief Returns the underlying SDL window handle.
 *
 * Provides access to the raw @p SDL_Window pointer for interoperability
 * with SDL APIs not wrapped by Cinder. Ownership remains with the engine —
 * do not destroy the returned pointer directly.
 *
 * @return  The underlying @p SDL_Window pointer.
 *
 * @see CinderGetSDLRenderer
 */
struct SDL_Window *CinderGetSDLWindow(void);

/**
 * @brief Returns the underlying SDL renderer handle.
 *
 * Provides access to the raw @p SDL_Renderer pointer for interoperability
 * with SDL APIs not wrapped by Cinder. Ownership remains with the engine —
 * do not destroy the returned pointer directly.
 *
 * @return  The underlying @p SDL_Renderer pointer.
 *
 * @see CinderGetSDLWindow
 */
struct SDL_Renderer *CinderGetSDLRenderer(void);

#endif /* CINDER_WINDOW_H_ */