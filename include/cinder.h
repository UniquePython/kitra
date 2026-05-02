#ifndef CINDER_H_
#define CINDER_H_

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>

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
    size_t width;
    size_t height;

    int x;
    int y;

    CinderWindowFlags flags;

} CinderWindowDesc;

#define CINDER_WINDOW_POS_CENTERED INT_MIN

CinderStatus CinderCreateWindow(const CinderWindowDesc *winDesc);

// ======================================= CORE LOOP ================================================

bool CinderIsRunning(void);
void CinderRequestQuit(void);

void CinderBeginFrame(void);
void CinderEndFrame(void);

// ======================================= COLOR ================================================

typedef struct CinderColor
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
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

// ======================================= ERROR ================================================

const char *CinderGetError(void);

#endif /* CINDER_H_ */