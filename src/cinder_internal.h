#ifndef CINDER_INTERNAL_H_
#define CINDER_INTERNAL_H_

#define _POSIX_C_SOURCE 200809L

#include "cinder.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Internal state of the Cinder library.
 *
 * Holds the core SDL2 objects and initialization flags used throughout
 * the library. This struct is managed internally by Cinder — do not
 * create or modify it directly.
 *
 * @note Access the window and renderer via CinderGetWindow() and
 *       CinderGetRenderer() if you need them for advanced SDL2 usage.
 */
typedef struct CinderCoreState
{
    SDL_Window *window;     /**< The SDL2 window handle. */
    SDL_Renderer *renderer; /**< The SDL2 renderer handle. */
    uint32_t initFlags;     /**< SDL subsystem flags passed to CinderInit(). */
    bool imgInitialized;    /**< Whether SDL2_image was successfully initialized. */
    bool ttfInitialized;    /**< Whether SDL2_ttf was successfully initialized. */
    bool audioInitialized;  /**< Whether SDL2_mixer was successfully initialized. */
} CinderCoreState;

/**
 * @brief Internal state of the Cinder game loop.
 *
 * Tracks the current frame and loop lifecycle. Managed internally —
 * do not create or modify it directly.
 */
typedef struct CinderLoopState
{
    bool isRunning;  /**< Whether the main loop is currently running. Set to false by CinderQuit(). */
    bool frameBegun; /**< Whether CinderBeginFrame() has been called without a matching CinderEndFrame(). */
    bool resized;    /**< Whether the window was resized this frame. */
} CinderLoopState;

/**
 * @brief Internal timing state used to track frame timing and FPS.
 *
 * Manages delta time, FPS calculation, and optional FPS capping.
 * Managed internally — do not create or modify it directly.
 */
typedef struct CinderTimingState
{
    uint64_t perfFrequency; /**< SDL2 performance counter frequency, used for high-resolution timing. */
    uint64_t lastCounter;   /**< Performance counter value at the start of the last frame. */
    float deltaTime;        /**< Time in seconds elapsed since the last frame. */
    float fps;              /**< Current frames per second, updated each frame. */
    int targetFPS;          /**< Target FPS cap. Set to 0 to disable frame rate limiting. */
} CinderTimingState;

/**
 * @brief Internal keyboard input state for the current frame.
 *
 * Tracks the per-frame state of every key. Updated at the start of
 * each frame by CinderBeginFrame(). Managed internally — do not
 * create or modify it directly.
 *
 * @see CinderIsKeyDown(), CinderIsKeyPressed(), CinderIsKeyReleased()
 */
typedef struct CinderKeyboardState
{
    bool down[CINDER_KEY_COUNT];     /**< True while the key is held down. */
    bool pressed[CINDER_KEY_COUNT];  /**< True only on the frame the key was first pressed. */
    bool released[CINDER_KEY_COUNT]; /**< True only on the frame the key was released. */
} CinderKeyboardState;

/**
 * @brief Internal mouse input state for the current frame.
 *
 * Tracks mouse position, movement, scroll, and button states.
 * Updated at the start of each frame by CinderBeginFrame().
 * Managed internally — do not create or modify it directly.
 *
 * @see CinderGetMousePos(), CinderIsMouseButtonDown(),
 *      CinderIsMouseButtonPressed(), CinderIsMouseButtonReleased()
 */
typedef struct CinderMouseState
{
    CinderPoint pos;                          /**< Current mouse position in window coordinates. */
    CinderPoint prevPos;                      /**< Mouse position during the previous frame. */
    CinderVec2i delta;                        /**< Mouse movement since the last frame. */
    CinderVec2i scrollDelta;                  /**< Scroll wheel delta this frame. */
    bool down[CINDER_MOUSE_BUTTON_COUNT];     /**< True while the button is held down. */
    bool pressed[CINDER_MOUSE_BUTTON_COUNT];  /**< True only on the frame the button was first pressed. */
    bool released[CINDER_MOUSE_BUTTON_COUNT]; /**< True only on the frame the button was released. */
} CinderMouseState;

/**
 * @brief Internal input state aggregating keyboard and mouse state.
 *
 * Combines keyboard and mouse state into a single struct. Updated at
 * the start of each frame by CinderBeginFrame(). Managed internally —
 * do not create or modify it directly.
 *
 * @see CinderKeyboardState, CinderMouseState
 */
typedef struct CinderInputState
{
    CinderKeyboardState keyboard; /**< Current keyboard input state. */
    CinderMouseState mouse;       /**< Current mouse input state. */
} CinderInputState;

/**
 * @brief Internal logging and error state.
 *
 * Tracks the user-provided log callback and the most recent error
 * message and severity. Managed internally — do not create or modify
 * it directly.
 *
 * @see CinderSetErrorCallback(), CinderGetLastError(), CinderLogLevel
 */
typedef struct CinderLogState
{
    CinderErrorCallback callback; /**< User-provided callback invoked on every log message. NULL if not set. */
    void *userdata;               /**< User-provided pointer passed through to the callback. */
    const char *lastError;        /**< The most recent error message, or NULL if no error has occurred. */
    CinderLogLevel lastLevel;     /**< Severity level of the most recent log message. */
} CinderLogState;

/**
 * @brief The global Cinder context.
 *
 * The single top-level struct that holds all library state — core SDL2
 * objects, loop control, timing, input, logging, RNG, and plugins.
 * There is exactly one instance of this struct for the lifetime of the
 * application. Managed internally — do not create or modify it directly.
 *
 * @see CinderInit(), CinderQuit()
 */
typedef struct CinderCtx
{
    CinderCoreState core;     /**< Core SDL2 window and renderer state. */
    CinderLoopState loop;     /**< Game loop lifecycle state. */
    CinderTimingState timing; /**< Frame timing, delta time and FPS state. */
    CinderInputState input;   /**< Keyboard and mouse input state. */
    CinderLogState log;       /**< Logging and error reporting state. */
    CinderRng rng;            /**< Global random number generator instance. */

    CinderPlugin plugins[CINDER_MAX_PLUGINS]; /**< Registered plugin instances, up to CINDER_MAX_PLUGINS. */
    int pluginCount;                          /**< Number of currently registered plugins. */
} CinderCtx;

extern CinderCtx gCinderCtx;

void CinderInputInit(void);
void CinderInputProcessEvent(const SDL_Event *event);

SDL_Texture *CinderTextureGetSDL(CinderTexture *tex);

#endif /* CINDER_INTERNAL_H_ */