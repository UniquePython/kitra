#ifndef CINDER_INTERNAL_H_
#define CINDER_INTERNAL_H_

#define _POSIX_C_SOURCE 200809L

#include "cinder.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct CinderCtx
{
    // ------ Core ------

    SDL_Window *window;
    SDL_Renderer *renderer;

    uint32_t initFlags;

    bool imgInitialized;
    bool ttfInitialized;

    // ------ Loop ------

    bool isRunning;
    bool frameBegun;

    // ------ Timing ------

    uint64_t perfFrequency;
    uint64_t lastCounter;

    float deltaTime;
    float fps;
    int targetFPS;

    // ------ Keyboard ------

    bool keysDown[CINDER_KEY_COUNT];
    bool keysPressed[CINDER_KEY_COUNT];

    // ------ Mouse ------

    CinderPoint mousePos;
    CinderPoint prevMousePos;
    CinderVec2i mouseDelta;
    CinderVec2i scrollDelta;

    bool mouseDown[CINDER_MOUSE_BUTTON_COUNT];
    bool mousePressed[CINDER_MOUSE_BUTTON_COUNT];

    // ------ Logging ------

    CinderErrorCallback callback;
    void *callbackUserdata;

    const char *lastError;
    CinderLogLevel lastLevel;

} CinderCtx;

extern CinderCtx gCinderCtx;

void CinderInputInit(void);
void CinderInputProcessEvent(const SDL_Event *event);

#endif /* CINDER_INTERNAL_H_ */