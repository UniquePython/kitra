#ifndef CINDER_H_
#define CINDER_H_

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>

#include "cinder_compiler.h"
#include "cinder_math.h"

typedef struct CinderCtx CinderCtx;

typedef enum
{
    CINDER_STATUS_OK,
    CINDER_STATUS_SUBSYSTEM_INIT_FAILED,
    CINDER_STATUS_WINDOW_CREATE_FAILED,
    CINDER_STATUS_RENDERER_CREATE_FAILED,

} CinderStatus;

// ======================================= SUBSYSTEM ================================================

typedef enum
{
    CINDER_SUBSYSTEM_NONE = 0,

    CINDER_SUBSYSTEM_EVENTS = 1u << 0,
    CINDER_SUBSYSTEM_AUDIO = 1u << 1,
    CINDER_SUBSYSTEM_VIDEO = 1u << 2,
    CINDER_SUBSYSTEM_ALL = CINDER_SUBSYSTEM_EVENTS | CINDER_SUBSYSTEM_AUDIO | CINDER_SUBSYSTEM_VIDEO

} CinderSubsystem;

CINDER_WARN_UNUSED_RESULT
CinderStatus CinderInit(CinderSubsystem flags);
void CinderQuit(void);

// ======================================= WINDOW ================================================

typedef enum
{
    CINDER_WINDOW_NONE = 0,

    CINDER_WINDOW_SHOWN = 1u << 0,
    CINDER_WINDOW_RESIZABLE = 1u << 1,
    CINDER_WINDOW_FULLSCREEN = 1u << 2,
    CINDER_WINDOW_BORDERLESS = 1u << 3,
    CINDER_WINDOW_HIDDEN = 1u << 4,

} CinderWindowFlags;

typedef struct CinderWindowDesc
{
    const char *title;
    int width;
    int height;

    int x;
    int y;

    CinderWindowFlags flags;

} CinderWindowDesc;

#define CINDER_WINDOW_POS_CENTERED INT_MIN

CINDER_WARN_UNUSED_RESULT
CinderStatus CinderCreateWindow(const CinderWindowDesc *winDesc);
void CinderDestroyWindow(void);

// ======================================= INPUT HANDLING ================================================

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

    CINDER_KEY_COUNT = 100
} CinderKey;

bool CinderIsKeyDown(CinderKey key);
bool CinderIsKeyPressed(CinderKey key);

// ======================================= CORE LOOP ================================================

bool CinderIsRunning(void);
void CinderRequestQuit(void);

void CinderBeginFrame(void);
void CinderEndFrame(void);

float CinderGetDeltaTime(void);

// ======================================= COLOR ================================================

typedef struct CinderColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} CinderColor;

#define CINDER_COLOR_RGBA(r, g, b, a) \
    (CinderColor) { r, g, b, a }

#define CINDER_COLOR_RGB(r, g, b) (CinderColor){r, g, b, 255}

extern const CinderColor CINDER_BLACK;
extern const CinderColor CINDER_WHITE;
extern const CinderColor CINDER_RED;
extern const CinderColor CINDER_GREEN;
extern const CinderColor CINDER_BLUE;
extern const CinderColor CINDER_YELLOW;
extern const CinderColor CINDER_MAGENTA;
extern const CinderColor CINDER_CYAN;

// ======================================= DRAWING ================================================

void CinderClearBackground(CinderColor color);

// --------------------------------------- LINE ---------------------------------------

void CinderDrawLine(int x1, int y1, int x2, int y2, CinderColor color);
void CinderDrawLineV(CinderVec2i start, CinderVec2i end, CinderColor color);

// --------------------------------------- CIRCLE ---------------------------------------

void CinderDrawCircle(int x, int y, int radius, CinderColor color);
void CinderDrawCircleV(CinderVec2i pos, int radius, CinderColor color);

void CinderDrawCircleOutline(int x, int y, int radius, CinderColor color);
void CinderDrawCircleOutlineV(CinderVec2i pos, int radius, CinderColor color);

// --------------------------------------- TRIANGLE ---------------------------------------

void CinderDrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, CinderColor color);
void CinderDrawTriangleV(CinderVec2i p1, CinderVec2i p2, CinderVec2i p3, CinderColor color);

void CinderDrawTriangleOutline(int x1, int y1, int x2, int y2, int x3, int y3, CinderColor color);
void CinderDrawTriangleOutlineV(CinderVec2i p1, CinderVec2i p2, CinderVec2i p3, CinderColor color);

// --------------------------------------- RECTANGLE ---------------------------------------

void CinderDrawRect(CinderRect rect, CinderColor color);
void CinderDrawRectOutline(CinderRect rect, CinderColor color);

void CinderDrawRoundedRect(CinderRect rect, int radius, CinderColor color);
void CinderDrawRoundedRectOutline(CinderRect rect, int radius, CinderColor color);

// ======================================= TEXTURE ================================================

typedef struct CinderTexture CinderTexture;

CINDER_WARN_UNUSED_RESULT
CINDER_NONNULL(1)
CinderTexture *CinderLoadTexture(const char *path);

CINDER_NONNULL(1)
void CinderDrawTextureEx(CinderTexture *tex, const CinderRect *src, const CinderRect *dst);

CINDER_NONNULL(1)
void CinderDrawTexture(CinderTexture *tex, int x, int y);

CINDER_NONNULL(1)
void CinderDrawTextureV(CinderTexture *tex, CinderVec2i pos);

void CinderDestroyTexture(CinderTexture **tex);

// ======================================= ERROR ================================================

typedef enum
{
    CINDER_LOG_INFO,
    CINDER_LOG_WARNING,
    CINDER_LOG_ERROR,

} CinderLogLevel;

typedef void (*CinderErrorCallback)(
    CinderLogLevel level,
    const char *message,
    const char *file,
    const char *function,
    int line,
    void *userdata);

void CinderSetErrorCallback(CinderErrorCallback cb, void *userdata);

CINDER_WARN_UNUSED_RESULT
const char *CinderGetError(void);

CINDER_WARN_UNUSED_RESULT
CinderLogLevel CinderGetLastLevel(void);

bool CinderHasError(void);

void CinderLogInternal__(CinderLogLevel level, const char *msg, const char *file, const char *function, int line);

void CinderDefaultLogCallback(CinderLogLevel level, const char *message, const char *file, const char *function, int line, void *userdata);

#define CINDER_LOG(level, msg) CinderLogInternal__((level), (msg), __FILE__, __func__, __LINE__)

#endif /* CINDER_H_ */