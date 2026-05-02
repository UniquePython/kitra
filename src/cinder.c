#include "cinder.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string.h>

typedef struct CinderCtx
{
    uint32_t initFlags;

    SDL_Window *window;
    SDL_Renderer *renderer;

    bool isRunning;
    bool frameBegun;
    bool imgInitialized;

    bool keysDown[CINDER_KEY_COUNT];
    bool keysPressed[CINDER_KEY_COUNT];

    uint64_t lastCounter;
    uint64_t perfFrequency;
    float deltaTime;

    const char *errMsg;

} CinderCtx;

CinderCtx gCinderCtx;

// ======================================= SUBSYSTEM ================================================

static void CinderRollbackSubsystems(uint32_t flags)
{
    if (flags & SDL_INIT_VIDEO)
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    if (flags & SDL_INIT_AUDIO)
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    if (flags & SDL_INIT_EVENTS)
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

CinderStatus CinderInit(CinderSubsystem flags)
{
    gCinderCtx.initFlags = CINDER_SUBSYSTEM_NONE;

    gCinderCtx.window = NULL;
    gCinderCtx.renderer = NULL;

    gCinderCtx.isRunning = false;
    gCinderCtx.frameBegun = false;
    gCinderCtx.imgInitialized = false;

    memset(gCinderCtx.keysDown, false, sizeof(gCinderCtx.keysDown));
    memset(gCinderCtx.keysPressed, false, sizeof(gCinderCtx.keysPressed));

    gCinderCtx.lastCounter = 0;
    gCinderCtx.perfFrequency = SDL_GetPerformanceFrequency();
    gCinderCtx.deltaTime = 0.0f;

    if (flags & CINDER_SUBSYSTEM_EVENTS)
    {
        if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
        {
            gCinderCtx.errMsg = "Failed to initialize EVENTS subsystem";

            CinderRollbackSubsystems(gCinderCtx.initFlags);
            gCinderCtx.initFlags = 0;

            return CINDER_STATUS_SUBSYSTEM_INIT_FAILED;
        }
        gCinderCtx.initFlags |= SDL_INIT_EVENTS;
    }

    if (flags & CINDER_SUBSYSTEM_AUDIO)
    {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
        {
            gCinderCtx.errMsg = "Failed to initialize AUDIO subsystem";

            CinderRollbackSubsystems(gCinderCtx.initFlags);
            gCinderCtx.initFlags = 0;

            return CINDER_STATUS_SUBSYSTEM_INIT_FAILED;
        }
        gCinderCtx.initFlags |= SDL_INIT_AUDIO;
    }

    if (flags & CINDER_SUBSYSTEM_VIDEO)
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            gCinderCtx.errMsg = "Failed to initialize VIDEO subsystem";

            CinderRollbackSubsystems(gCinderCtx.initFlags);
            gCinderCtx.initFlags = 0;

            return CINDER_STATUS_SUBSYSTEM_INIT_FAILED;
        }
        gCinderCtx.initFlags |= SDL_INIT_VIDEO;
    }

    return CINDER_STATUS_OK;
}

// Forward declare destruction functions
void CinderDestroyWindow(void);

void CinderQuit(void)
{
    CinderDestroyWindow();

    gCinderCtx.initFlags = 0;

    if (gCinderCtx.imgInitialized)
        IMG_Quit();

    SDL_Quit();
}

// ======================================= WINDOW ================================================

static uint32_t CinderToSDLWindowFlags(CinderWindowFlags flags)
{
    uint32_t sdlFlags = 0;

    if (flags & CINDER_WINDOW_SHOWN)
        sdlFlags |= SDL_WINDOW_SHOWN;

    if (flags & CINDER_WINDOW_RESIZABLE)
        sdlFlags |= SDL_WINDOW_RESIZABLE;

    if (flags & CINDER_WINDOW_FULLSCREEN)
        sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    if (flags & CINDER_WINDOW_BORDERLESS)
        sdlFlags |= SDL_WINDOW_BORDERLESS;

    if (flags & CINDER_WINDOW_HIDDEN)
        sdlFlags |= SDL_WINDOW_HIDDEN;

    return sdlFlags;
}

CinderStatus CinderCreateWindow(const CinderWindowDesc *winDesc)
{
    CinderWindowDesc internalWinDesc = {0};

    if (!winDesc)
    {
        internalWinDesc.title = "Cinder";
        internalWinDesc.width = 900;
        internalWinDesc.height = 600;
        internalWinDesc.x = SDL_WINDOWPOS_CENTERED;
        internalWinDesc.y = SDL_WINDOWPOS_CENTERED;
        internalWinDesc.flags = CINDER_WINDOW_SHOWN | CINDER_WINDOW_RESIZABLE;
    }
    else
    {
        internalWinDesc.title = winDesc->title;
        internalWinDesc.width = winDesc->width;
        internalWinDesc.height = winDesc->height;
        internalWinDesc.x = winDesc->x;
        internalWinDesc.y = winDesc->y;
        internalWinDesc.flags = winDesc->flags;
    }

    gCinderCtx.window = SDL_CreateWindow(
        internalWinDesc.title,
        internalWinDesc.x == CINDER_WINDOW_POS_CENTERED ? SDL_WINDOWPOS_CENTERED : internalWinDesc.x,
        internalWinDesc.y == CINDER_WINDOW_POS_CENTERED ? SDL_WINDOWPOS_CENTERED : internalWinDesc.y,
        (int)internalWinDesc.width, (int)internalWinDesc.height,
        CinderToSDLWindowFlags(internalWinDesc.flags));

    if (!gCinderCtx.window)
    {
        gCinderCtx.errMsg = SDL_GetError();
        return CINDER_STATUS_WINDOW_CREATE_FAILED;
    }

    gCinderCtx.renderer = SDL_CreateRenderer(gCinderCtx.window, -1, SDL_RENDERER_ACCELERATED);

    if (!gCinderCtx.renderer)
    {
        gCinderCtx.errMsg = SDL_GetError();

        SDL_DestroyWindow(gCinderCtx.window);
        gCinderCtx.window = NULL;

        return CINDER_STATUS_RENDERER_CREATE_FAILED;
    }

    gCinderCtx.isRunning = true;

    return CINDER_STATUS_OK;
}

void CinderDestroyWindow(void)
{
    if (gCinderCtx.renderer)
    {
        SDL_DestroyRenderer(gCinderCtx.renderer);
        gCinderCtx.renderer = NULL;
    }

    if (gCinderCtx.window)
    {
        SDL_DestroyWindow(gCinderCtx.window);
        gCinderCtx.window = NULL;
    }
}

// ======================================= INPUT HANDLING ================================================

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

bool CinderIsKeyDown(CinderKey key)
{
    if (key < 0 || key >= CINDER_KEY_COUNT)
        return false;

    return gCinderCtx.keysDown[key];
}

bool CinderIsKeyPressed(CinderKey key)
{
    if (key < 0 || key >= CINDER_KEY_COUNT)
        return false;

    return gCinderCtx.keysPressed[key];
}

// ======================================= CORE LOOP ================================================

bool CinderIsRunning(void)
{
    return gCinderCtx.isRunning;
}

void CinderRequestQuit(void)
{
    gCinderCtx.isRunning = false;
}

void CinderBeginFrame(void)
{
    // ---------------- Delta Time ----------------
    uint64_t now = SDL_GetPerformanceCounter();

    if (gCinderCtx.lastCounter == 0)
        gCinderCtx.deltaTime = 0.0f;
    else
        gCinderCtx.deltaTime = (float)(now - gCinderCtx.lastCounter) / (float)gCinderCtx.perfFrequency;

    gCinderCtx.lastCounter = now;

    // ---------------- Reset per-frame input ----------------

    memset(gCinderCtx.keysPressed, false, sizeof(gCinderCtx.keysPressed));

    gCinderCtx.frameBegun = true;

    // ---------------- Event processing ----------------

    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            CinderRequestQuit();
            break;

        case SDL_KEYDOWN:
            if (!sdlEvent.key.repeat)
            {
                SDL_Scancode sc = sdlEvent.key.keysym.scancode;

                for (int i = 0; i < CINDER_KEY_COUNT; i++)
                {
                    if (sCinderKeyToSDL[i] == sc)
                    {
                        gCinderCtx.keysDown[i] = true;
                        gCinderCtx.keysPressed[i] = true;
                        break;
                    }
                }
            }
            break;

        case SDL_KEYUP:
        {
            SDL_Scancode sc = sdlEvent.key.keysym.scancode;

            for (int i = 0; i < CINDER_KEY_COUNT; i++)
            {
                if (sCinderKeyToSDL[i] == sc)
                {
                    gCinderCtx.keysDown[i] = false;
                    break;
                }
            }
        }
        break;

        default:
            break;
        }
    }
}

void CinderEndFrame(void)
{
    if (!gCinderCtx.frameBegun)
    {
        gCinderCtx.errMsg = "CinderEndFrame called without CinderBeginFrame";
        return;
    }

    gCinderCtx.frameBegun = false;

    if (gCinderCtx.renderer)
    {
        SDL_RenderPresent(gCinderCtx.renderer);
    }
    else
    {
        gCinderCtx.errMsg = "Renderer is NULL in CinderEndFrame";
    }
}

float CinderGetDeltaTime(void)
{
    return gCinderCtx.deltaTime;
}

// ======================================= COLOR ================================================

const CinderColor CINDER_BLACK = CINDER_COLOR_RGB(0, 0, 0);
const CinderColor CINDER_WHITE = CINDER_COLOR_RGB(255, 255, 255);
const CinderColor CINDER_RED = CINDER_COLOR_RGB(255, 0, 0);
const CinderColor CINDER_GREEN = CINDER_COLOR_RGB(0, 255, 0);
const CinderColor CINDER_BLUE = CINDER_COLOR_RGB(0, 0, 255);
const CinderColor CINDER_YELLOW = CINDER_COLOR_RGB(255, 255, 0);
const CinderColor CINDER_MAGENTA = CINDER_COLOR_RGB(255, 0, 255);
const CinderColor CINDER_CYAN = CINDER_COLOR_RGB(0, 255, 255);

// ======================================= DRAWING ================================================

void CinderClearBackground(CinderColor color)
{
    if (!gCinderCtx.renderer)
    {
        gCinderCtx.errMsg = "Renderer is NULL in CinderClearBackground";
        return;
    }

    SDL_SetRenderDrawColor(gCinderCtx.renderer, color.r, color.g, color.b, color.a);

    SDL_RenderClear(gCinderCtx.renderer);
}

// --------------------------------------- RECTANGLE ---------------------------------------

static SDL_Rect CinderToSDLRect(CinderRect rect)
{
    return (SDL_Rect){rect.x, rect.y, rect.w, rect.h};
}

void CinderDrawRect(CinderRect rect, CinderColor color)
{
    if (!gCinderCtx.renderer)
        return;

    SDL_Rect sdlRect = CinderToSDLRect(rect);

    SDL_SetRenderDrawColor(gCinderCtx.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(gCinderCtx.renderer, &sdlRect);
}

void CinderDrawRectOutline(CinderRect rect, CinderColor color)
{
    if (!gCinderCtx.renderer)
        return;

    SDL_Rect sdlRect = CinderToSDLRect(rect);

    SDL_SetRenderDrawColor(gCinderCtx.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(gCinderCtx.renderer, &sdlRect);
}

// ======================================= TEXTURE ================================================

typedef struct CinderTexture
{
    SDL_Texture *handle;

    int width, height;

} CinderTexture;

CinderTexture *CinderLoadTexture(const char *path)
{
    if (!gCinderCtx.imgInitialized)
    {
        int flags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP | IMG_INIT_TIF | IMG_INIT_JXL | IMG_INIT_AVIF;
        int impFormats = IMG_INIT_PNG | IMG_INIT_JPG;

        int initted = IMG_Init(flags);
        if ((initted & impFormats) != impFormats)
        {
            gCinderCtx.errMsg = IMG_GetError();
            gCinderCtx.imgInitialized = false;
            return NULL;
        }
        gCinderCtx.imgInitialized = true;
    }

    if (!gCinderCtx.renderer)
    {
        gCinderCtx.errMsg = "Renderer not initialized";
        return NULL;
    }

    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        gCinderCtx.errMsg = IMG_GetError();
        return NULL;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(gCinderCtx.renderer, surface);
    if (!tex)
    {
        gCinderCtx.errMsg = SDL_GetError();
        SDL_FreeSurface(surface);
        return NULL;
    }

    CinderTexture *cTex = malloc(sizeof(CinderTexture));
    if (!cTex)
    {
        gCinderCtx.errMsg = "Failed to allocate CinderTexture";
        SDL_DestroyTexture(tex);
        SDL_FreeSurface(surface);
        return NULL;
    }

    cTex->handle = tex;

    SDL_FreeSurface(surface);
    SDL_QueryTexture(tex, NULL, NULL, &cTex->width, &cTex->height);

    return cTex;
}

void CinderDrawTextureEx(CinderTexture *tex, const CinderRect *src, const CinderRect *dst)
{
    if (!gCinderCtx.renderer || !tex)
        return;

    SDL_Rect sdlSrc;
    SDL_Rect sdlDst;

    SDL_Rect *pSrc = NULL;
    SDL_Rect *pDst = NULL;

    if (src)
    {
        sdlSrc.x = src->x;
        sdlSrc.y = src->y;
        sdlSrc.w = (int)src->w;
        sdlSrc.h = (int)src->h;
        pSrc = &sdlSrc;
    }

    if (dst)
    {
        sdlDst.x = dst->x;
        sdlDst.y = dst->y;
        sdlDst.w = (int)dst->w;
        sdlDst.h = (int)dst->h;
        pDst = &sdlDst;
    }

    SDL_RenderCopy(gCinderCtx.renderer, tex->handle, pSrc, pDst);
}

void CinderDrawTexture(CinderTexture *tex, int x, int y)
{
    if (!tex)
        return;

    CinderRect dst = {x, y, tex->width, tex->height};

    CinderDrawTextureEx(tex, NULL, &dst);
}

void CinderDestroyTexture(CinderTexture **tex)
{
    if (!tex || !*tex)
        return;

    if ((*tex)->handle)
        SDL_DestroyTexture((*tex)->handle);

    free(*tex);
    *tex = NULL;
}

// ======================================= ERROR ================================================

const char *CinderGetError(void)
{
    return gCinderCtx.errMsg;
}
