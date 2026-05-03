#ifndef CINDER_SUBSYSTEM_H_
#define CINDER_SUBSYSTEM_H_

#include "cinder_status.h"

typedef enum
{
    CINDER_SUBSYSTEM_NONE = 0,
    CINDER_SUBSYSTEM_EVENTS = 1u << 0,
    CINDER_SUBSYSTEM_AUDIO = 1u << 1,
    CINDER_SUBSYSTEM_VIDEO = 1u << 2,
    CINDER_SUBSYSTEM_ALL = CINDER_SUBSYSTEM_EVENTS | CINDER_SUBSYSTEM_AUDIO | CINDER_SUBSYSTEM_VIDEO,

} CinderSubsystem;

CinderStatus CinderInit(CinderSubsystem flags);
void CinderQuit(void);

#endif /* CINDER_SUBSYSTEM_H_ */