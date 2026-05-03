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
#include <stdbool.h>

typedef struct CinderCoreState
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    uint32_t initFlags;
    bool imgInitialized;
    bool ttfInitialized;
} CinderCoreState;

typedef struct CinderLoopState
{
    bool isRunning;
    bool frameBegun;
} CinderLoopState;

typedef struct CinderTimingState
{
    uint64_t perfFrequency;
    uint64_t lastCounter;
    float deltaTime;
    float fps;
    int targetFPS;
} CinderTimingState;

typedef struct CinderKeyboardState
{
    bool down[CINDER_KEY_COUNT];
    bool pressed[CINDER_KEY_COUNT];
} CinderKeyboardState;

typedef struct CinderMouseState
{
    CinderPoint pos;
    CinderPoint prevPos;
    CinderVec2i delta;
    CinderVec2i scrollDelta;
    bool down[CINDER_MOUSE_BUTTON_COUNT];
    bool pressed[CINDER_MOUSE_BUTTON_COUNT];
} CinderMouseState;

typedef struct CinderInputState
{
    CinderKeyboardState keyboard;
    CinderMouseState mouse;
} CinderInputState;

typedef struct CinderLogState
{
    CinderErrorCallback callback;
    void *userdata;
    const char *lastError;
    CinderLogLevel lastLevel;
} CinderLogState;

typedef struct CinderCtx
{
    CinderCoreState core;
    CinderLoopState loop;
    CinderTimingState timing;
    CinderInputState input;
    CinderLogState log;
} CinderCtx;

extern CinderCtx gCinderCtx;

void CinderInputInit(void);
void CinderInputProcessEvent(const SDL_Event *event);

#endif /* CINDER_INTERNAL_H_ */