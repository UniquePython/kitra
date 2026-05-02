#ifndef CINDER_H_
#define CINDER_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct s_cinder_ctx CinderCtx;

typedef enum
{
    CINDER_STATUS_OK,
    CINDER_STATUS_SUBSYSTEM_INIT_FAILED,
} CinderStatus;

typedef enum
{
    CINDER_SUBSYSTEM_NONE = 0,

    CINDER_SUBSYSTEM_EVENTS = 1u << 0, // 0x01
    CINDER_SUBSYSTEM_AUDIO = 1u << 1,  // 0x02
    CINDER_SUBSYSTEM_VIDEO = 1u << 2,  // 0x04
    CINDER_SUBSYSTEM_ALL = CINDER_SUBSYSTEM_EVENTS | CINDER_SUBSYSTEM_AUDIO | CINDER_SUBSYSTEM_VIDEO
} CinderSubsystem;

CinderStatus CinderInit(CinderSubsystem flags);
void CinderQuit(void);

CinderStatus CinderCreateWindow(const char *title, size_t width, size_t height);
CinderStatus CinderDestroyWindow(void);

const char *CinderGetError(void);

#endif /* CINDER_H_ */