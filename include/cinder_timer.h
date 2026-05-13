#ifndef CINDER_TIMER_H_
#define CINDER_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct CinderTimer
{
    uint64_t start;
    uint64_t last;
    bool repeat;
} CinderTimer;

/**
 * @brief Creates and starts a new timer.
 *
 * Captures the current performance counter as both the start and last
 * tick, with repeat mode disabled.
 *
 * @return  A freshly started @p CinderTimer.
 *
 * @see CinderResetTimer, CinderGetTimerElapsed, CinderTimerDone
 */
CinderTimer CinderStartTimer(void);

/**
 * @brief Resets a timer back to the current moment.
 *
 * Updates both the start and last tick to now, effectively restarting
 * the timer without changing its repeat setting. Does nothing if @p t
 * is @p NULL.
 *
 * @param t  Timer to reset.
 *
 * @see CinderStartTimer, CinderGetTimerElapsed
 */
void CinderResetTimer(CinderTimer *t);

/**
 * @brief Returns the total elapsed time since the timer was started or reset.
 *
 * @param t  Timer to query.
 * @return   Seconds elapsed since @p t was started or last reset, or
 *           @p 0.0f if @p t is @p NULL.
 *
 * @see CinderStartTimer, CinderResetTimer, CinderTimerDone
 */
float CinderGetTimerElapsed(const CinderTimer *t);

/**
 * @brief Returns whether a timer has reached the given duration.
 *
 * Checks whether at least @p duration seconds have elapsed since the timer's
 * last tick. If repeat mode is enabled, the last tick is advanced by
 * @p duration on expiry rather than reset to now, preserving sub-frame
 * accuracy for fixed-interval callbacks. Does nothing and returns @p false
 * if @p t is @p NULL.
 *
 * @param t         Timer to check.
 * @param duration  Duration threshold in seconds.
 * @return          @p true if @p duration seconds have elapsed since the
 *                  last tick, @p false otherwise.
 *
 * @see CinderTimerSetRepeat, CinderGetTimerElapsed
 */
bool CinderTimerDone(CinderTimer *t, float duration);

/**
 * @brief Sets whether a timer automatically repeats.
 *
 * When repeat mode is enabled, @p CinderTimerDone advances the internal
 * last-tick timestamp on expiry rather than requiring a manual reset,
 * making it suitable for fixed-interval callbacks. Does nothing if @p t
 * is @p NULL.
 *
 * @param t       Timer to modify.
 * @param repeat  If @p true, the timer repeats; if @p false, it fires once.
 *
 * @see CinderTimerDone
 */
void CinderTimerSetRepeat(CinderTimer *t, bool repeat);

/**
 * @brief Returns the duration of the last frame in seconds.
 *
 * Useful for frame-rate-independent movement and animation. Updated once
 * per frame by the engine's main loop.
 *
 * @return  Delta time of the last frame in seconds.
 *
 * @see CinderSetTargetFPS, CinderGetFPS
 */
float CinderGetDeltaTime(void);

/**
 * @brief Sets the target frame rate of the engine's main loop.
 *
 * The engine will attempt to cap the frame rate at @p fps by sleeping at
 * the end of each frame. Pass @p 0 to disable the cap and run unconstrained.
 *
 * @param fps  Desired frame rate in frames per second.
 *
 * @see CinderGetFPS, CinderGetDeltaTime
 */
void CinderSetTargetFPS(int fps);

/**
 * @brief Returns the current measured frame rate.
 *
 * @return  Frames per second as measured over recent frames.
 *
 * @see CinderSetTargetFPS, CinderGetDeltaTime
 */
float CinderGetFPS(void);

#endif /* CINDER_TIMER_H_ */