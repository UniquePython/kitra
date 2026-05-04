#ifndef CINDER_INPUT_H_
#define CINDER_INPUT_H_

#include "cinder_types.h"

#include <stdbool.h>

typedef enum CinderKey
{
    CINDER_KEY_UNKNOWN = 0,

    CINDER_KEY_A = 1,
    CINDER_KEY_B = 2,
    CINDER_KEY_C = 3,
    CINDER_KEY_D = 4,
    CINDER_KEY_E = 5,
    CINDER_KEY_F = 6,
    CINDER_KEY_G = 7,
    CINDER_KEY_H = 8,
    CINDER_KEY_I = 9,
    CINDER_KEY_J = 10,
    CINDER_KEY_K = 11,
    CINDER_KEY_L = 12,
    CINDER_KEY_M = 13,
    CINDER_KEY_N = 14,
    CINDER_KEY_O = 15,
    CINDER_KEY_P = 16,
    CINDER_KEY_Q = 17,
    CINDER_KEY_R = 18,
    CINDER_KEY_S = 19,
    CINDER_KEY_T = 20,
    CINDER_KEY_U = 21,
    CINDER_KEY_V = 22,
    CINDER_KEY_W = 23,
    CINDER_KEY_X = 24,
    CINDER_KEY_Y = 25,
    CINDER_KEY_Z = 26,

    CINDER_KEY_0 = 27,
    CINDER_KEY_1 = 28,
    CINDER_KEY_2 = 29,
    CINDER_KEY_3 = 30,
    CINDER_KEY_4 = 31,
    CINDER_KEY_5 = 32,
    CINDER_KEY_6 = 33,
    CINDER_KEY_7 = 34,
    CINDER_KEY_8 = 35,
    CINDER_KEY_9 = 36,

    CINDER_KEY_F1 = 37,
    CINDER_KEY_F2 = 38,
    CINDER_KEY_F3 = 39,
    CINDER_KEY_F4 = 40,
    CINDER_KEY_F5 = 41,
    CINDER_KEY_F6 = 42,
    CINDER_KEY_F7 = 43,
    CINDER_KEY_F8 = 44,
    CINDER_KEY_F9 = 45,
    CINDER_KEY_F10 = 46,
    CINDER_KEY_F11 = 47,
    CINDER_KEY_F12 = 48,

    CINDER_KEY_UP = 49,
    CINDER_KEY_DOWN = 50,
    CINDER_KEY_LEFT = 51,
    CINDER_KEY_RIGHT = 52,
    CINDER_KEY_HOME = 53,
    CINDER_KEY_END = 54,
    CINDER_KEY_PAGEUP = 55,
    CINDER_KEY_PAGEDOWN = 56,
    CINDER_KEY_INSERT = 57,
    CINDER_KEY_DELETE = 58,

    CINDER_KEY_SPACE = 59,
    CINDER_KEY_ENTER = 60,
    CINDER_KEY_ESCAPE = 61,
    CINDER_KEY_BACKSPACE = 62,
    CINDER_KEY_TAB = 63,

    CINDER_KEY_LSHIFT = 64,
    CINDER_KEY_RSHIFT = 65,
    CINDER_KEY_LCTRL = 66,
    CINDER_KEY_RCTRL = 67,
    CINDER_KEY_LALT = 68,
    CINDER_KEY_RALT = 69,
    CINDER_KEY_CAPSLOCK = 70,

    CINDER_KEY_MINUS = 71,
    CINDER_KEY_EQUALS = 72,
    CINDER_KEY_LEFTBRACKET = 73,
    CINDER_KEY_RIGHTBRACKET = 74,
    CINDER_KEY_BACKSLASH = 75,
    CINDER_KEY_SEMICOLON = 76,
    CINDER_KEY_APOSTROPHE = 77,
    CINDER_KEY_GRAVE = 78,
    CINDER_KEY_COMMA = 79,
    CINDER_KEY_PERIOD = 80,
    CINDER_KEY_SLASH = 81,

    CINDER_KEY_KP0 = 82,
    CINDER_KEY_KP1 = 83,
    CINDER_KEY_KP2 = 84,
    CINDER_KEY_KP3 = 85,
    CINDER_KEY_KP4 = 86,
    CINDER_KEY_KP5 = 87,
    CINDER_KEY_KP6 = 88,
    CINDER_KEY_KP7 = 89,
    CINDER_KEY_KP8 = 90,
    CINDER_KEY_KP9 = 91,
    CINDER_KEY_KP_ENTER = 92,
    CINDER_KEY_KP_PLUS = 93,
    CINDER_KEY_KP_MINUS = 94,
    CINDER_KEY_KP_MULTIPLY = 95,
    CINDER_KEY_KP_DIVIDE = 96,

    CINDER_KEY_PRINTSCREEN = 97,
    CINDER_KEY_SCROLLLOCK = 98,
    CINDER_KEY_PAUSE = 99,

    CINDER_KEY_COUNT

} CinderKey;

typedef enum CinderMouseButton
{
    CINDER_MOUSE_LEFT = 0,
    CINDER_MOUSE_RIGHT = 1,
    CINDER_MOUSE_MIDDLE = 2,
    CINDER_MOUSE_SIDE1 = 3,
    CINDER_MOUSE_SIDE2 = 4,

    CINDER_MOUSE_BUTTON_COUNT

} CinderMouseButton;

bool CinderIsKeyDown(CinderKey key);
bool CinderIsKeyPressed(CinderKey key);
bool CinderIsKeyReleased(CinderKey key);

CinderPoint CinderGetMousePos(void);
CinderVec2i CinderGetMouseDelta(void);
CinderVec2i CinderGetScrollDelta(void);

bool CinderIsMouseButtonDown(CinderMouseButton button);
bool CinderIsMouseButtonPressed(CinderMouseButton button);
bool CinderIsMouseButtonReleased(CinderMouseButton button);

#endif /* CINDER_INPUT_H_ */