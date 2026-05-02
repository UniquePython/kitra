#ifndef CINDER_H_
#define CINDER_H_

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

typedef struct s_cinder_ctx CinderCtx;

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
void CinderDestroyWindow(void);

// ======================================= CORE LOOP ================================================

bool CinderIsRunning(void);
void CinderRequestQuit(void);

void CinderBeginFrame(void);
void CinderEndFrame(void);

// ======================================= ERROR ================================================

const char *CinderGetError(void);

#endif /* CINDER_H_ */