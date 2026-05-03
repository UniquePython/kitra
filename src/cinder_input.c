#include "cinder_internal.h"

static SDL_Scancode sCinderKeyToSDL[CINDER_KEY_COUNT] = {
    [CINDER_KEY_UNKNOWN] = SDL_SCANCODE_UNKNOWN,
    [CINDER_KEY_A] = SDL_SCANCODE_A,
    [CINDER_KEY_B] = SDL_SCANCODE_B,
    [CINDER_KEY_C] = SDL_SCANCODE_C,
    [CINDER_KEY_D] = SDL_SCANCODE_D,
    [CINDER_KEY_E] = SDL_SCANCODE_E,
    [CINDER_KEY_F] = SDL_SCANCODE_F,
    [CINDER_KEY_G] = SDL_SCANCODE_G,
    [CINDER_KEY_H] = SDL_SCANCODE_H,
    [CINDER_KEY_I] = SDL_SCANCODE_I,
    [CINDER_KEY_J] = SDL_SCANCODE_J,
    [CINDER_KEY_K] = SDL_SCANCODE_K,
    [CINDER_KEY_L] = SDL_SCANCODE_L,
    [CINDER_KEY_M] = SDL_SCANCODE_M,
    [CINDER_KEY_N] = SDL_SCANCODE_N,
    [CINDER_KEY_O] = SDL_SCANCODE_O,
    [CINDER_KEY_P] = SDL_SCANCODE_P,
    [CINDER_KEY_Q] = SDL_SCANCODE_Q,
    [CINDER_KEY_R] = SDL_SCANCODE_R,
    [CINDER_KEY_S] = SDL_SCANCODE_S,
    [CINDER_KEY_T] = SDL_SCANCODE_T,
    [CINDER_KEY_U] = SDL_SCANCODE_U,
    [CINDER_KEY_V] = SDL_SCANCODE_V,
    [CINDER_KEY_W] = SDL_SCANCODE_W,
    [CINDER_KEY_X] = SDL_SCANCODE_X,
    [CINDER_KEY_Y] = SDL_SCANCODE_Y,
    [CINDER_KEY_Z] = SDL_SCANCODE_Z,
    [CINDER_KEY_0] = SDL_SCANCODE_0,
    [CINDER_KEY_1] = SDL_SCANCODE_1,
    [CINDER_KEY_2] = SDL_SCANCODE_2,
    [CINDER_KEY_3] = SDL_SCANCODE_3,
    [CINDER_KEY_4] = SDL_SCANCODE_4,
    [CINDER_KEY_5] = SDL_SCANCODE_5,
    [CINDER_KEY_6] = SDL_SCANCODE_6,
    [CINDER_KEY_7] = SDL_SCANCODE_7,
    [CINDER_KEY_8] = SDL_SCANCODE_8,
    [CINDER_KEY_9] = SDL_SCANCODE_9,
    [CINDER_KEY_F1] = SDL_SCANCODE_F1,
    [CINDER_KEY_F2] = SDL_SCANCODE_F2,
    [CINDER_KEY_F3] = SDL_SCANCODE_F3,
    [CINDER_KEY_F4] = SDL_SCANCODE_F4,
    [CINDER_KEY_F5] = SDL_SCANCODE_F5,
    [CINDER_KEY_F6] = SDL_SCANCODE_F6,
    [CINDER_KEY_F7] = SDL_SCANCODE_F7,
    [CINDER_KEY_F8] = SDL_SCANCODE_F8,
    [CINDER_KEY_F9] = SDL_SCANCODE_F9,
    [CINDER_KEY_F10] = SDL_SCANCODE_F10,
    [CINDER_KEY_F11] = SDL_SCANCODE_F11,
    [CINDER_KEY_F12] = SDL_SCANCODE_F12,
    [CINDER_KEY_UP] = SDL_SCANCODE_UP,
    [CINDER_KEY_DOWN] = SDL_SCANCODE_DOWN,
    [CINDER_KEY_LEFT] = SDL_SCANCODE_LEFT,
    [CINDER_KEY_RIGHT] = SDL_SCANCODE_RIGHT,
    [CINDER_KEY_HOME] = SDL_SCANCODE_HOME,
    [CINDER_KEY_END] = SDL_SCANCODE_END,
    [CINDER_KEY_PAGEUP] = SDL_SCANCODE_PAGEUP,
    [CINDER_KEY_PAGEDOWN] = SDL_SCANCODE_PAGEDOWN,
    [CINDER_KEY_INSERT] = SDL_SCANCODE_INSERT,
    [CINDER_KEY_DELETE] = SDL_SCANCODE_DELETE,
    [CINDER_KEY_SPACE] = SDL_SCANCODE_SPACE,
    [CINDER_KEY_ENTER] = SDL_SCANCODE_RETURN,
    [CINDER_KEY_ESCAPE] = SDL_SCANCODE_ESCAPE,
    [CINDER_KEY_BACKSPACE] = SDL_SCANCODE_BACKSPACE,
    [CINDER_KEY_TAB] = SDL_SCANCODE_TAB,
    [CINDER_KEY_LSHIFT] = SDL_SCANCODE_LSHIFT,
    [CINDER_KEY_RSHIFT] = SDL_SCANCODE_RSHIFT,
    [CINDER_KEY_LCTRL] = SDL_SCANCODE_LCTRL,
    [CINDER_KEY_RCTRL] = SDL_SCANCODE_RCTRL,
    [CINDER_KEY_LALT] = SDL_SCANCODE_LALT,
    [CINDER_KEY_RALT] = SDL_SCANCODE_RALT,
    [CINDER_KEY_CAPSLOCK] = SDL_SCANCODE_CAPSLOCK,
    [CINDER_KEY_MINUS] = SDL_SCANCODE_MINUS,
    [CINDER_KEY_EQUALS] = SDL_SCANCODE_EQUALS,
    [CINDER_KEY_LEFTBRACKET] = SDL_SCANCODE_LEFTBRACKET,
    [CINDER_KEY_RIGHTBRACKET] = SDL_SCANCODE_RIGHTBRACKET,
    [CINDER_KEY_BACKSLASH] = SDL_SCANCODE_BACKSLASH,
    [CINDER_KEY_SEMICOLON] = SDL_SCANCODE_SEMICOLON,
    [CINDER_KEY_APOSTROPHE] = SDL_SCANCODE_APOSTROPHE,
    [CINDER_KEY_GRAVE] = SDL_SCANCODE_GRAVE,
    [CINDER_KEY_COMMA] = SDL_SCANCODE_COMMA,
    [CINDER_KEY_PERIOD] = SDL_SCANCODE_PERIOD,
    [CINDER_KEY_SLASH] = SDL_SCANCODE_SLASH,
    [CINDER_KEY_KP0] = SDL_SCANCODE_KP_0,
    [CINDER_KEY_KP1] = SDL_SCANCODE_KP_1,
    [CINDER_KEY_KP2] = SDL_SCANCODE_KP_2,
    [CINDER_KEY_KP3] = SDL_SCANCODE_KP_3,
    [CINDER_KEY_KP4] = SDL_SCANCODE_KP_4,
    [CINDER_KEY_KP5] = SDL_SCANCODE_KP_5,
    [CINDER_KEY_KP6] = SDL_SCANCODE_KP_6,
    [CINDER_KEY_KP7] = SDL_SCANCODE_KP_7,
    [CINDER_KEY_KP8] = SDL_SCANCODE_KP_8,
    [CINDER_KEY_KP9] = SDL_SCANCODE_KP_9,
    [CINDER_KEY_KP_ENTER] = SDL_SCANCODE_KP_ENTER,
    [CINDER_KEY_KP_PLUS] = SDL_SCANCODE_KP_PLUS,
    [CINDER_KEY_KP_MINUS] = SDL_SCANCODE_KP_MINUS,
    [CINDER_KEY_KP_MULTIPLY] = SDL_SCANCODE_KP_MULTIPLY,
    [CINDER_KEY_KP_DIVIDE] = SDL_SCANCODE_KP_DIVIDE,
    [CINDER_KEY_PRINTSCREEN] = SDL_SCANCODE_PRINTSCREEN,
    [CINDER_KEY_SCROLLLOCK] = SDL_SCANCODE_SCROLLLOCK,
    [CINDER_KEY_PAUSE] = SDL_SCANCODE_PAUSE,
};

static CinderKey sSdlToCinderKey[SDL_NUM_SCANCODES];

void CinderInputInit(void)
{
    memset(sSdlToCinderKey, 0, sizeof(sSdlToCinderKey));
    for (int i = 0; i < CINDER_KEY_COUNT; i++)
    {
        SDL_Scancode sc = sCinderKeyToSDL[i];
        if (sc != SDL_SCANCODE_UNKNOWN)
            sSdlToCinderKey[sc] = (CinderKey)i;
    }
}

void CinderInputProcessEvent(const SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_KEYDOWN:
        if (!event->key.repeat)
        {
            CinderKey key = sSdlToCinderKey[event->key.keysym.scancode];
            if (key != CINDER_KEY_UNKNOWN)
            {
                gCinderCtx.input.keyboard.down[key] = true;
                gCinderCtx.input.keyboard.pressed[key] = true;
            }
        }
        break;

    case SDL_KEYUP:
    {
        CinderKey key = sSdlToCinderKey[event->key.keysym.scancode];
        if (key != CINDER_KEY_UNKNOWN)
            gCinderCtx.input.keyboard.down[key] = false;
    }
    break;

    case SDL_MOUSEMOTION:
        gCinderCtx.input.mouse.pos.x = event->motion.x;
        gCinderCtx.input.mouse.pos.y = event->motion.y;
        gCinderCtx.input.mouse.delta.x += event->motion.xrel;
        gCinderCtx.input.mouse.delta.y += event->motion.yrel;
        break;

    case SDL_MOUSEBUTTONDOWN:
    {
        int index = event->button.button - 1;
        if (index >= 0 && index < CINDER_MOUSE_BUTTON_COUNT)
        {
            gCinderCtx.input.mouse.down[index] = true;
            gCinderCtx.input.mouse.pressed[index] = true;
        }
    }
    break;

    case SDL_MOUSEBUTTONUP:
    {
        int index = event->button.button - 1;
        if (index >= 0 && index < CINDER_MOUSE_BUTTON_COUNT)
            gCinderCtx.input.mouse.down[index] = false;
    }
    break;

    case SDL_MOUSEWHEEL:
        gCinderCtx.input.mouse.scrollDelta.x += event->wheel.x;
        gCinderCtx.input.mouse.scrollDelta.y += event->wheel.y;
        break;

    default:
        break;
    }
}

bool CinderIsKeyDown(CinderKey key)
{
    if (key < 0 || key >= CINDER_KEY_COUNT)
        return false;

    return gCinderCtx.input.keyboard.down[key];
}

bool CinderIsKeyPressed(CinderKey key)
{
    if (key < 0 || key >= CINDER_KEY_COUNT)
        return false;

    return gCinderCtx.input.keyboard.pressed[key];
}

CinderPoint CinderGetMousePos(void)
{
    return gCinderCtx.input.mouse.pos;
}

CinderVec2i CinderGetMouseDelta(void)
{
    return gCinderCtx.input.mouse.delta;
}

CinderVec2i CinderGetScrollDelta(void)
{
    return gCinderCtx.input.mouse.scrollDelta;
}

bool CinderIsMouseButtonDown(CinderMouseButton button)
{
    if (button < 0 || button >= CINDER_MOUSE_BUTTON_COUNT)
        return false;

    return gCinderCtx.input.mouse.down[button];
}

bool CinderIsMouseButtonPressed(CinderMouseButton button)
{
    if (button < 0 || button >= CINDER_MOUSE_BUTTON_COUNT)
        return false;

    return gCinderCtx.input.mouse.pressed[button];
}