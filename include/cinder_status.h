#ifndef CINDER_STATUS_H_
#define CINDER_STATUS_H_

/**
 * @brief Return codes used throughout the Cinder API.
 *
 * Every Cinder function that can fail and does not return a pointer, returns a CinderStatus value.
 * Check against CINDER_STATUS_OK to determine success.
 *
 * @see CinderGetLastError()
 */
typedef enum
{
    CINDER_STATUS_OK,                     /**< Operation completed successfully. */
    CINDER_STATUS_SUBSYSTEM_DOUBLE_INIT,  /**< CinderInit() was called more than once without CinderQuit(). */
    CINDER_STATUS_SUBSYSTEM_INIT_FAILED,  /**< An SDL2 subsystem failed to initialize. */
    CINDER_STATUS_WINDOW_CREATE_FAILED,   /**< The SDL2 window could not be created. */
    CINDER_STATUS_RENDERER_CREATE_FAILED, /**< The SDL2 renderer could not be created. */
    CINDER_STATUS_PLUGIN_MAX_REACHED,     /**< The plugin registry is full — CINDER_MAX_PLUGINS plugins are already registered. */
    CINDER_STATUS_PLUGIN_NO_NAME,         /**< A plugin was registered without a name. */
    CINDER_STATUS_PLUGIN_DUPLICATE_NAME,  /**< A plugin with the same name is already registered. */
    CINDER_STATUS_RENDERER_MISSING,       /**< An operation requiring a renderer was attempted before the renderer was created. */
    CINDER_STATUS_TEXTURE_NULL,           /**< A NULL texture was passed to a function that requires a valid texture. */
    CINDER_STATUS_SCREENSHOT_FAILED,      /**< The screenshot could not be saved to disk. */

} CinderStatus;

#endif /* CINDER_STATUS_H_ */