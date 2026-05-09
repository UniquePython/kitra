#ifndef CINDER_SUBSYSTEM_H_
#define CINDER_SUBSYSTEM_H_

#include "cinder_status.h"

/**
 * @brief Flags for selecting which Cinder subsystems to initialize.
 *
 * Pass one or more of these flags to CinderInit(), combined with the
 * bitwise OR operator. Use CINDER_SUBSYSTEM_ALL to initialize everything.
 *
 * @code
 * CinderInit(CINDER_SUBSYSTEM_VIDEO | CINDER_SUBSYSTEM_AUDIO);
 * @endcode
 *
 * @see CinderInit()
 */
typedef enum
{
    CINDER_SUBSYSTEM_NONE = 0,                                                                        /**< No subsystems. */
    CINDER_SUBSYSTEM_EVENTS = 1u << 0,                                                                /**< Event processing — required for input handling. */
    CINDER_SUBSYSTEM_AUDIO = 1u << 1,                                                                 /**< Audio — required for sound effects and music. */
    CINDER_SUBSYSTEM_VIDEO = 1u << 2,                                                                 /**< Video — required for window creation and rendering. */
    CINDER_SUBSYSTEM_ALL = CINDER_SUBSYSTEM_EVENTS | CINDER_SUBSYSTEM_AUDIO | CINDER_SUBSYSTEM_VIDEO, /**< All subsystems. */

} CinderSubsystem;

/**
 * @brief Initializes the Cinder library and the requested subsystems.
 *
 * Must be called once before any other Cinder function. Initializes
 * SDL2 subsystems based on the provided flags, seeds the global RNG,
 * and sets up the default log callback. Subsystems are initialized in
 * dependency order with rollback safety — if any subsystem fails, all
 * previously initialized subsystems are cleanly shut down before returning.
 *
 * @param flags One or more @ref CinderSubsystem flags combined with
 *              the bitwise OR operator. Use CINDER_SUBSYSTEM_ALL to
 *              initialize everything.
 *
 * @return CINDER_STATUS_OK on success.
 * @return CINDER_STATUS_SUBSYSTEM_DOUBLE_INIT if CinderInit() has
 *         already been called without a matching CinderQuit().
 * @return CINDER_STATUS_SUBSYSTEM_INIT_FAILED if any SDL2 subsystem
 *         or SDL2_mixer failed to initialize.
 *
 * @see CinderQuit(), CinderSubsystem
 */
CinderStatus CinderInit(CinderSubsystem flags);

/**
 * @brief Shuts down the Cinder library and frees all resources.
 *
 * Shuts down all active subsystems and releases all resources in the
 * correct order — plugins are shut down in reverse registration order,
 * followed by the window, renderer, and SDL2 subsystems. Should be
 * called once at the end of the program, after the main loop exits.
 *
 * @note Calling any Cinder function after CinderQuit() results in
 *       undefined behavior unless CinderInit() is called again first.
 *
 * @see CinderInit()
 */
void CinderQuit(void);

#endif /* CINDER_SUBSYSTEM_H_ */