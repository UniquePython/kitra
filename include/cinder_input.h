#ifndef CINDER_INPUT_H_
#define CINDER_INPUT_H_

#include "cinder_types.h"

#include <stdbool.h>

/**
 * @brief Keyboard key identifiers.
 *
 * Platform-independent key codes used throughout the input API. Values are
 * stable integers that can be used as array indices. @p CINDER_KEY_COUNT
 * gives the total number of tracked keys and is suitable for sizing
 * per-key arrays.
 *
 * @see CinderIsKeyDown, CinderIsKeyPressed, CinderIsKeyReleased
 */
typedef enum CinderKey
{
    CINDER_KEY_UNKNOWN = 0, /**< Unknown or unmapped key. */

    /* — Alphabet — */
    CINDER_KEY_A = 1,  /**< A key. */
    CINDER_KEY_B = 2,  /**< B key. */
    CINDER_KEY_C = 3,  /**< C key. */
    CINDER_KEY_D = 4,  /**< D key. */
    CINDER_KEY_E = 5,  /**< E key. */
    CINDER_KEY_F = 6,  /**< F key. */
    CINDER_KEY_G = 7,  /**< G key. */
    CINDER_KEY_H = 8,  /**< H key. */
    CINDER_KEY_I = 9,  /**< I key. */
    CINDER_KEY_J = 10, /**< J key. */
    CINDER_KEY_K = 11, /**< K key. */
    CINDER_KEY_L = 12, /**< L key. */
    CINDER_KEY_M = 13, /**< M key. */
    CINDER_KEY_N = 14, /**< N key. */
    CINDER_KEY_O = 15, /**< O key. */
    CINDER_KEY_P = 16, /**< P key. */
    CINDER_KEY_Q = 17, /**< Q key. */
    CINDER_KEY_R = 18, /**< R key. */
    CINDER_KEY_S = 19, /**< S key. */
    CINDER_KEY_T = 20, /**< T key. */
    CINDER_KEY_U = 21, /**< U key. */
    CINDER_KEY_V = 22, /**< V key. */
    CINDER_KEY_W = 23, /**< W key. */
    CINDER_KEY_X = 24, /**< X key. */
    CINDER_KEY_Y = 25, /**< Y key. */
    CINDER_KEY_Z = 26, /**< Z key. */

    /* — Digits — */
    CINDER_KEY_0 = 27, /**< 0 key. */
    CINDER_KEY_1 = 28, /**< 1 key. */
    CINDER_KEY_2 = 29, /**< 2 key. */
    CINDER_KEY_3 = 30, /**< 3 key. */
    CINDER_KEY_4 = 31, /**< 4 key. */
    CINDER_KEY_5 = 32, /**< 5 key. */
    CINDER_KEY_6 = 33, /**< 6 key. */
    CINDER_KEY_7 = 34, /**< 7 key. */
    CINDER_KEY_8 = 35, /**< 8 key. */
    CINDER_KEY_9 = 36, /**< 9 key. */

    /* — Function keys — */
    CINDER_KEY_F1 = 37,  /**< F1 key. */
    CINDER_KEY_F2 = 38,  /**< F2 key. */
    CINDER_KEY_F3 = 39,  /**< F3 key. */
    CINDER_KEY_F4 = 40,  /**< F4 key. */
    CINDER_KEY_F5 = 41,  /**< F5 key. */
    CINDER_KEY_F6 = 42,  /**< F6 key. */
    CINDER_KEY_F7 = 43,  /**< F7 key. */
    CINDER_KEY_F8 = 44,  /**< F8 key. */
    CINDER_KEY_F9 = 45,  /**< F9 key. */
    CINDER_KEY_F10 = 46, /**< F10 key. */
    CINDER_KEY_F11 = 47, /**< F11 key. */
    CINDER_KEY_F12 = 48, /**< F12 key. */

    /* — Navigation — */
    CINDER_KEY_UP = 49,       /**< Up arrow key. */
    CINDER_KEY_DOWN = 50,     /**< Down arrow key. */
    CINDER_KEY_LEFT = 51,     /**< Left arrow key. */
    CINDER_KEY_RIGHT = 52,    /**< Right arrow key. */
    CINDER_KEY_HOME = 53,     /**< Home key. */
    CINDER_KEY_END = 54,      /**< End key. */
    CINDER_KEY_PAGEUP = 55,   /**< Page Up key. */
    CINDER_KEY_PAGEDOWN = 56, /**< Page Down key. */
    CINDER_KEY_INSERT = 57,   /**< Insert key. */
    CINDER_KEY_DELETE = 58,   /**< Delete key. */

    /* — Control — */
    CINDER_KEY_SPACE = 59,     /**< Space bar. */
    CINDER_KEY_ENTER = 60,     /**< Enter / Return key. */
    CINDER_KEY_ESCAPE = 61,    /**< Escape key. */
    CINDER_KEY_BACKSPACE = 62, /**< Backspace key. */
    CINDER_KEY_TAB = 63,       /**< Tab key. */

    /* — Modifiers — */
    CINDER_KEY_LSHIFT = 64,   /**< Left Shift key. */
    CINDER_KEY_RSHIFT = 65,   /**< Right Shift key. */
    CINDER_KEY_LCTRL = 66,    /**< Left Control key. */
    CINDER_KEY_RCTRL = 67,    /**< Right Control key. */
    CINDER_KEY_LALT = 68,     /**< Left Alt key. */
    CINDER_KEY_RALT = 69,     /**< Right Alt key. */
    CINDER_KEY_CAPSLOCK = 70, /**< Caps Lock key. */

    /* — Punctuation & symbols — */
    CINDER_KEY_MINUS = 71,        /**< Minus / Hyphen key (@p -). */
    CINDER_KEY_EQUALS = 72,       /**< Equals key (@p =). */
    CINDER_KEY_LEFTBRACKET = 73,  /**< Left bracket key (@p [). */
    CINDER_KEY_RIGHTBRACKET = 74, /**< Right bracket key (@p ]). */
    CINDER_KEY_BACKSLASH = 75,    /**< Backslash key (@p \\). */
    CINDER_KEY_SEMICOLON = 76,    /**< Semicolon key (@p ;). */
    CINDER_KEY_APOSTROPHE = 77,   /**< Apostrophe key (@p '). */
    CINDER_KEY_GRAVE = 78,        /**< Grave accent / backtick key (@p `). */
    CINDER_KEY_COMMA = 79,        /**< Comma key (@p ,). */
    CINDER_KEY_PERIOD = 80,       /**< Period key (@p .). */
    CINDER_KEY_SLASH = 81,        /**< Forward slash key (@p /). */

    /* — Keypad — */
    CINDER_KEY_KP0 = 82,         /**< Keypad 0. */
    CINDER_KEY_KP1 = 83,         /**< Keypad 1. */
    CINDER_KEY_KP2 = 84,         /**< Keypad 2. */
    CINDER_KEY_KP3 = 85,         /**< Keypad 3. */
    CINDER_KEY_KP4 = 86,         /**< Keypad 4. */
    CINDER_KEY_KP5 = 87,         /**< Keypad 5. */
    CINDER_KEY_KP6 = 88,         /**< Keypad 6. */
    CINDER_KEY_KP7 = 89,         /**< Keypad 7. */
    CINDER_KEY_KP8 = 90,         /**< Keypad 8. */
    CINDER_KEY_KP9 = 91,         /**< Keypad 9. */
    CINDER_KEY_KP_ENTER = 92,    /**< Keypad Enter. */
    CINDER_KEY_KP_PLUS = 93,     /**< Keypad Plus (@p +). */
    CINDER_KEY_KP_MINUS = 94,    /**< Keypad Minus (@p -). */
    CINDER_KEY_KP_MULTIPLY = 95, /**< Keypad Multiply (@p *). */
    CINDER_KEY_KP_DIVIDE = 96,   /**< Keypad Divide (@p /). */

    /* — System — */
    CINDER_KEY_PRINTSCREEN = 97, /**< Print Screen key. */
    CINDER_KEY_SCROLLLOCK = 98,  /**< Scroll Lock key. */
    CINDER_KEY_PAUSE = 99,       /**< Pause / Break key. */

    CINDER_KEY_COUNT /**< Total number of tracked keys — use for sizing per-key arrays. */

} CinderKey;

/**
 * @brief Mouse button identifiers.
 *
 * Platform-independent button codes used throughout the input API. Values are
 * stable integers that can be used as array indices. @p CINDER_MOUSE_BUTTON_COUNT
 * gives the total number of tracked buttons and is suitable for sizing
 * per-button arrays.
 *
 * @see CinderIsMouseButtonDown, CinderIsMouseButtonPressed, CinderIsMouseButtonReleased
 */
typedef enum CinderMouseButton
{
    CINDER_MOUSE_LEFT = 0,   /**< Primary (left) mouse button. */
    CINDER_MOUSE_RIGHT = 1,  /**< Secondary (right) mouse button. */
    CINDER_MOUSE_MIDDLE = 2, /**< Middle mouse button / scroll wheel click. */
    CINDER_MOUSE_SIDE1 = 3,  /**< First side button (typically Back). */
    CINDER_MOUSE_SIDE2 = 4,  /**< Second side button (typically Forward). */

    CINDER_MOUSE_BUTTON_COUNT /**< Total number of tracked buttons — use for sizing per-button arrays. */

} CinderMouseButton;

/**
 * @brief Returns whether a key is currently held down.
 *
 * @param key  Key to query.
 * @return     @p true if @p key is held this frame, @p false otherwise or
 *             if @p key is out of range.
 *
 * @see CinderIsKeyPressed, CinderIsKeyReleased, CinderKey
 */
bool CinderIsKeyDown(CinderKey key);

/**
 * @brief Returns whether a key was pressed this frame.
 *
 * Only @p true on the single frame the key transitions from up to down.
 * Use @p CinderIsKeyDown for continuous held-key detection.
 *
 * @param key  Key to query.
 * @return     @p true if @p key was pressed this frame, @p false otherwise
 *             or if @p key is out of range.
 *
 * @see CinderIsKeyDown, CinderIsKeyReleased, CinderKey
 */
bool CinderIsKeyPressed(CinderKey key);

/**
 * @brief Returns whether a key was released this frame.
 *
 * Only @p true on the single frame the key transitions from down to up.
 *
 * @param key  Key to query.
 * @return     @p true if @p key was released this frame, @p false otherwise
 *             or if @p key is out of range.
 *
 * @see CinderIsKeyDown, CinderIsKeyPressed, CinderKey
 */
bool CinderIsKeyReleased(CinderKey key);

/**
 * @brief Returns the current mouse cursor position.
 *
 * Position is in screen coordinates relative to the top-left corner of
 * the window.
 *
 * @return  Current cursor position as a @p CinderPoint.
 *
 * @see CinderGetMouseDelta, CinderGetScrollDelta
 */
CinderPoint CinderGetMousePos(void);

/**
 * @brief Returns the mouse cursor movement since the last frame.
 *
 * Useful for camera controls or any logic that responds to relative
 * mouse motion rather than absolute position.
 *
 * @return  Per-frame cursor delta as a @p CinderVec2i.
 *
 * @see CinderGetMousePos, CinderGetScrollDelta
 */
CinderVec2i CinderGetMouseDelta(void);

/**
 * @brief Returns the scroll wheel delta since the last frame.
 *
 * The x component represents horizontal scroll and the y component
 * represents vertical scroll, where positive y is scroll up.
 *
 * @return  Per-frame scroll delta as a @p CinderVec2i.
 *
 * @see CinderGetMousePos, CinderGetMouseDelta
 */
CinderVec2i CinderGetScrollDelta(void);

/**
 * @brief Returns whether a mouse button is currently held down.
 *
 * @param button  Button to query.
 * @return        @p true if @p button is held this frame, @p false otherwise
 *                or if @p button is out of range.
 *
 * @see CinderIsMouseButtonPressed, CinderIsMouseButtonReleased, CinderMouseButton
 */
bool CinderIsMouseButtonDown(CinderMouseButton button);

/**
 * @brief Returns whether a mouse button was pressed this frame.
 *
 * Only @p true on the single frame the button transitions from up to down.
 * Use @p CinderIsMouseButtonDown for continuous held-button detection.
 *
 * @param button  Button to query.
 * @return        @p true if @p button was pressed this frame, @p false otherwise
 *                or if @p button is out of range.
 *
 * @see CinderIsMouseButtonDown, CinderIsMouseButtonReleased, CinderMouseButton
 */
bool CinderIsMouseButtonPressed(CinderMouseButton button);

/**
 * @brief Returns whether a mouse button was released this frame.
 *
 * Only @p true on the single frame the button transitions from down to up.
 *
 * @param button  Button to query.
 * @return        @p true if @p button was released this frame, @p false otherwise
 *                or if @p button is out of range.
 *
 * @see CinderIsMouseButtonDown, CinderIsMouseButtonPressed, CinderMouseButton
 */
bool CinderIsMouseButtonReleased(CinderMouseButton button);

#endif /* CINDER_INPUT_H_ */