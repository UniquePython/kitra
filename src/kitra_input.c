#include "kitra_internal.h"

static SDL_Scancode sKitraKeyToSDL[KITRA_KEY_COUNT] = {
    [KITRA_KEY_UNKNOWN] = SDL_SCANCODE_UNKNOWN,
    [KITRA_KEY_A] = SDL_SCANCODE_A,
    [KITRA_KEY_B] = SDL_SCANCODE_B,
    [KITRA_KEY_C] = SDL_SCANCODE_C,
    [KITRA_KEY_D] = SDL_SCANCODE_D,
    [KITRA_KEY_E] = SDL_SCANCODE_E,
    [KITRA_KEY_F] = SDL_SCANCODE_F,
    [KITRA_KEY_G] = SDL_SCANCODE_G,
    [KITRA_KEY_H] = SDL_SCANCODE_H,
    [KITRA_KEY_I] = SDL_SCANCODE_I,
    [KITRA_KEY_J] = SDL_SCANCODE_J,
    [KITRA_KEY_K] = SDL_SCANCODE_K,
    [KITRA_KEY_L] = SDL_SCANCODE_L,
    [KITRA_KEY_M] = SDL_SCANCODE_M,
    [KITRA_KEY_N] = SDL_SCANCODE_N,
    [KITRA_KEY_O] = SDL_SCANCODE_O,
    [KITRA_KEY_P] = SDL_SCANCODE_P,
    [KITRA_KEY_Q] = SDL_SCANCODE_Q,
    [KITRA_KEY_R] = SDL_SCANCODE_R,
    [KITRA_KEY_S] = SDL_SCANCODE_S,
    [KITRA_KEY_T] = SDL_SCANCODE_T,
    [KITRA_KEY_U] = SDL_SCANCODE_U,
    [KITRA_KEY_V] = SDL_SCANCODE_V,
    [KITRA_KEY_W] = SDL_SCANCODE_W,
    [KITRA_KEY_X] = SDL_SCANCODE_X,
    [KITRA_KEY_Y] = SDL_SCANCODE_Y,
    [KITRA_KEY_Z] = SDL_SCANCODE_Z,
    [KITRA_KEY_0] = SDL_SCANCODE_0,
    [KITRA_KEY_1] = SDL_SCANCODE_1,
    [KITRA_KEY_2] = SDL_SCANCODE_2,
    [KITRA_KEY_3] = SDL_SCANCODE_3,
    [KITRA_KEY_4] = SDL_SCANCODE_4,
    [KITRA_KEY_5] = SDL_SCANCODE_5,
    [KITRA_KEY_6] = SDL_SCANCODE_6,
    [KITRA_KEY_7] = SDL_SCANCODE_7,
    [KITRA_KEY_8] = SDL_SCANCODE_8,
    [KITRA_KEY_9] = SDL_SCANCODE_9,
    [KITRA_KEY_F1] = SDL_SCANCODE_F1,
    [KITRA_KEY_F2] = SDL_SCANCODE_F2,
    [KITRA_KEY_F3] = SDL_SCANCODE_F3,
    [KITRA_KEY_F4] = SDL_SCANCODE_F4,
    [KITRA_KEY_F5] = SDL_SCANCODE_F5,
    [KITRA_KEY_F6] = SDL_SCANCODE_F6,
    [KITRA_KEY_F7] = SDL_SCANCODE_F7,
    [KITRA_KEY_F8] = SDL_SCANCODE_F8,
    [KITRA_KEY_F9] = SDL_SCANCODE_F9,
    [KITRA_KEY_F10] = SDL_SCANCODE_F10,
    [KITRA_KEY_F11] = SDL_SCANCODE_F11,
    [KITRA_KEY_F12] = SDL_SCANCODE_F12,
    [KITRA_KEY_UP] = SDL_SCANCODE_UP,
    [KITRA_KEY_DOWN] = SDL_SCANCODE_DOWN,
    [KITRA_KEY_LEFT] = SDL_SCANCODE_LEFT,
    [KITRA_KEY_RIGHT] = SDL_SCANCODE_RIGHT,
    [KITRA_KEY_HOME] = SDL_SCANCODE_HOME,
    [KITRA_KEY_END] = SDL_SCANCODE_END,
    [KITRA_KEY_PAGEUP] = SDL_SCANCODE_PAGEUP,
    [KITRA_KEY_PAGEDOWN] = SDL_SCANCODE_PAGEDOWN,
    [KITRA_KEY_INSERT] = SDL_SCANCODE_INSERT,
    [KITRA_KEY_DELETE] = SDL_SCANCODE_DELETE,
    [KITRA_KEY_SPACE] = SDL_SCANCODE_SPACE,
    [KITRA_KEY_ENTER] = SDL_SCANCODE_RETURN,
    [KITRA_KEY_ESCAPE] = SDL_SCANCODE_ESCAPE,
    [KITRA_KEY_BACKSPACE] = SDL_SCANCODE_BACKSPACE,
    [KITRA_KEY_TAB] = SDL_SCANCODE_TAB,
    [KITRA_KEY_LSHIFT] = SDL_SCANCODE_LSHIFT,
    [KITRA_KEY_RSHIFT] = SDL_SCANCODE_RSHIFT,
    [KITRA_KEY_LCTRL] = SDL_SCANCODE_LCTRL,
    [KITRA_KEY_RCTRL] = SDL_SCANCODE_RCTRL,
    [KITRA_KEY_LALT] = SDL_SCANCODE_LALT,
    [KITRA_KEY_RALT] = SDL_SCANCODE_RALT,
    [KITRA_KEY_CAPSLOCK] = SDL_SCANCODE_CAPSLOCK,
    [KITRA_KEY_MINUS] = SDL_SCANCODE_MINUS,
    [KITRA_KEY_EQUALS] = SDL_SCANCODE_EQUALS,
    [KITRA_KEY_LEFTBRACKET] = SDL_SCANCODE_LEFTBRACKET,
    [KITRA_KEY_RIGHTBRACKET] = SDL_SCANCODE_RIGHTBRACKET,
    [KITRA_KEY_BACKSLASH] = SDL_SCANCODE_BACKSLASH,
    [KITRA_KEY_SEMICOLON] = SDL_SCANCODE_SEMICOLON,
    [KITRA_KEY_APOSTROPHE] = SDL_SCANCODE_APOSTROPHE,
    [KITRA_KEY_GRAVE] = SDL_SCANCODE_GRAVE,
    [KITRA_KEY_COMMA] = SDL_SCANCODE_COMMA,
    [KITRA_KEY_PERIOD] = SDL_SCANCODE_PERIOD,
    [KITRA_KEY_SLASH] = SDL_SCANCODE_SLASH,
    [KITRA_KEY_KP0] = SDL_SCANCODE_KP_0,
    [KITRA_KEY_KP1] = SDL_SCANCODE_KP_1,
    [KITRA_KEY_KP2] = SDL_SCANCODE_KP_2,
    [KITRA_KEY_KP3] = SDL_SCANCODE_KP_3,
    [KITRA_KEY_KP4] = SDL_SCANCODE_KP_4,
    [KITRA_KEY_KP5] = SDL_SCANCODE_KP_5,
    [KITRA_KEY_KP6] = SDL_SCANCODE_KP_6,
    [KITRA_KEY_KP7] = SDL_SCANCODE_KP_7,
    [KITRA_KEY_KP8] = SDL_SCANCODE_KP_8,
    [KITRA_KEY_KP9] = SDL_SCANCODE_KP_9,
    [KITRA_KEY_KP_ENTER] = SDL_SCANCODE_KP_ENTER,
    [KITRA_KEY_KP_PLUS] = SDL_SCANCODE_KP_PLUS,
    [KITRA_KEY_KP_MINUS] = SDL_SCANCODE_KP_MINUS,
    [KITRA_KEY_KP_MULTIPLY] = SDL_SCANCODE_KP_MULTIPLY,
    [KITRA_KEY_KP_DIVIDE] = SDL_SCANCODE_KP_DIVIDE,
    [KITRA_KEY_PRINTSCREEN] = SDL_SCANCODE_PRINTSCREEN,
    [KITRA_KEY_SCROLLLOCK] = SDL_SCANCODE_SCROLLLOCK,
    [KITRA_KEY_PAUSE] = SDL_SCANCODE_PAUSE,
};

static KitraKey sSdlToKitraKey[SDL_NUM_SCANCODES];

void KitraInputInit(void)
{
    memset(sSdlToKitraKey, 0, sizeof(sSdlToKitraKey));
    for (int i = 0; i < KITRA_KEY_COUNT; i++)
    {
        SDL_Scancode sc = sKitraKeyToSDL[i];
        if (sc != SDL_SCANCODE_UNKNOWN)
            sSdlToKitraKey[sc] = (KitraKey)i;
    }
}

void KitraInputProcessEvent(const SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_KEYDOWN:
        if (!event->key.repeat)
        {
            KitraKey key = sSdlToKitraKey[event->key.keysym.scancode];
            if (key != KITRA_KEY_UNKNOWN)
            {
                gKitraCtx.input.keyboard.down[key] = true;
                gKitraCtx.input.keyboard.pressed[key] = true;
            }
        }
        break;

    case SDL_KEYUP:
    {
        KitraKey key = sSdlToKitraKey[event->key.keysym.scancode];
        if (key != KITRA_KEY_UNKNOWN)
        {
            gKitraCtx.input.keyboard.down[key] = false;
            gKitraCtx.input.keyboard.released[key] = true;
        }
    }
    break;

    case SDL_MOUSEMOTION:
        gKitraCtx.input.mouse.pos.x = event->motion.x;
        gKitraCtx.input.mouse.pos.y = event->motion.y;
        gKitraCtx.input.mouse.delta.x += event->motion.xrel;
        gKitraCtx.input.mouse.delta.y += event->motion.yrel;
        break;

    case SDL_MOUSEBUTTONDOWN:
    {
        int index = event->button.button - 1;
        if (index >= 0 && index < KITRA_MOUSE_BUTTON_COUNT)
        {
            gKitraCtx.input.mouse.down[index] = true;
            gKitraCtx.input.mouse.pressed[index] = true;
        }
    }
    break;

    case SDL_MOUSEBUTTONUP:
    {
        int index = event->button.button - 1;
        if (index >= 0 && index < KITRA_MOUSE_BUTTON_COUNT)
        {
            gKitraCtx.input.mouse.down[index] = false;
            gKitraCtx.input.mouse.released[index] = true;
        }
    }
    break;

    case SDL_MOUSEWHEEL:
        gKitraCtx.input.mouse.scrollDelta.x += event->wheel.x;
        gKitraCtx.input.mouse.scrollDelta.y += event->wheel.y;
        break;

    case SDL_WINDOWEVENT:
        if (event->window.event == SDL_WINDOWEVENT_RESIZED)
            gKitraCtx.loop.resized = true;
        break;

    default:
        break;
    }
}

bool KitraIsKeyDown(KitraKey key)
{
    if (key < 0 || key >= KITRA_KEY_COUNT)
        return false;

    return gKitraCtx.input.keyboard.down[key];
}

bool KitraIsKeyPressed(KitraKey key)
{
    if (key < 0 || key >= KITRA_KEY_COUNT)
        return false;

    return gKitraCtx.input.keyboard.pressed[key];
}

bool KitraIsKeyReleased(KitraKey key)
{
    if (key < 0 || key >= KITRA_KEY_COUNT)
        return false;

    return gKitraCtx.input.keyboard.released[key];
}

KitraPoint KitraGetMousePos(void)
{
    return gKitraCtx.input.mouse.pos;
}

KitraVec2i KitraGetMouseDelta(void)
{
    return gKitraCtx.input.mouse.delta;
}

KitraVec2i KitraGetScrollDelta(void)
{
    return gKitraCtx.input.mouse.scrollDelta;
}

bool KitraIsMouseButtonDown(KitraMouseButton button)
{
    if (button < 0 || button >= KITRA_MOUSE_BUTTON_COUNT)
        return false;

    return gKitraCtx.input.mouse.down[button];
}

bool KitraIsMouseButtonPressed(KitraMouseButton button)
{
    if (button < 0 || button >= KITRA_MOUSE_BUTTON_COUNT)
        return false;

    return gKitraCtx.input.mouse.pressed[button];
}

bool KitraIsMouseButtonReleased(KitraMouseButton button)
{
    if (button < 0 || button >= KITRA_MOUSE_BUTTON_COUNT)
        return false;

    return gKitraCtx.input.mouse.released[button];
}