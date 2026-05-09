#ifndef CINDER_LOOP_H_
#define CINDER_LOOP_H_

#include <stdbool.h>

/**
 * @brief Returns whether the main loop should continue running.
 *
 * Typically used as the condition of the main game loop. Returns false
 * after CinderQuit() is called or the window is closed by the user.
 *
 * @return True if the loop is running, false if it should exit.
 *
 * @code
 * while (CinderIsRunning())
 * {
 *     CinderBeginFrame();
 *     // ...
 *     CinderEndFrame();
 * }
 * @endcode
 *
 * @see CinderBeginFrame(), CinderEndFrame()
 */
bool CinderIsRunning(void);

/**
 * @brief Requests the main loop to stop after the current frame.
 *
 * Sets the running flag to false, causing CinderIsRunning() to return
 * false on the next check. Does not immediately stop execution —
 * the current frame completes normally before the loop exits.
 *
 * @see CinderIsRunning(), CinderQuit()
 */
void CinderRequestQuit(void);

/**
 * @brief Begins a new frame.
 *
 * Must be called once at the start of every iteration of the main loop,
 * before any drawing or input queries. Performs the following in order:
 *
 * -# Updates delta time and smoothed FPS using an exponential moving average.
 * -# Resets per-frame input state (pressed, released, mouse delta, scroll).
 * -# Processes all pending SDL2 events, triggering CinderRequestQuit()
 *    on a window close event.
 * -# Calls the update callback of every registered plugin.
 *
 * @note Calling input functions or draw functions before CinderBeginFrame()
 *       will produce stale or undefined results.
 *
 * @see CinderEndFrame(), CinderIsRunning(), CinderGetDeltaTime()
 */
void CinderBeginFrame(void);

/**
 * @brief Ends the current frame and presents the rendered output.
 *
 * Must be called once at the end of every iteration of the main loop,
 * after all drawing is complete. Performs the following in order:
 *
 * -# Calls the draw callback of every registered plugin.
 * -# If a target FPS is set, sleeps for the remaining frame time to
 *    cap the frame rate.
 * -# Presents the renderer's back buffer to the screen.
 *
 * Logs a warning and returns early if called without a matching
 * CinderBeginFrame(), or if the renderer is NULL.
 *
 * @see CinderBeginFrame(), CinderSetTargetFPS()
 */
void CinderEndFrame(void);

#endif /* CINDER_LOOP_H_ */