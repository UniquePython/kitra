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
    uint32_t initFlags;

    SDL_Window *window;
    SDL_Renderer *renderer;

    bool isRunning;
    bool frameBegun;

    bool imgInitialized;
    bool ttfInitialized;

    // ------ Input handling ------

    bool keysDown[CINDER_KEY_COUNT];
    bool keysPressed[CINDER_KEY_COUNT];

    CinderPoint mousePos;
    CinderPoint prevMousePos;

    CinderVec2i mouseDelta;
    CinderVec2i scrollDelta;

    bool mouseDown[CINDER_MOUSE_BUTTON_COUNT];
    bool mousePressed[CINDER_MOUSE_BUTTON_COUNT];

    // ------ Delta time (raw) ------

    uint64_t lastCounter;
    uint64_t perfFrequency;
    float deltaTime;
    int targetFPS;
    float fps;

    // ------ Logging ------

    const char *lastError;
    CinderLogLevel lastLevel;

    CinderErrorCallback callback;
    void *callbackUserdata;

} CinderCtx;

extern CinderCtx gCinderCtx;

void CinderInputInit(void);
void CinderInputProcessEvent(const SDL_Event *event);

#endif /* CINDER_INTERNAL_H_ */