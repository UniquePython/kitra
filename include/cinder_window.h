#ifndef CINDER_WINDOW_H_
#define CINDER_WINDOW_H_

#include "cinder_types.h"
#include "cinder_status.h"

#include <stdbool.h>

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
    CinderSize size;
    CinderPoint pos; // ignored if centerX/centerY is true

    bool centerX;
    bool centerY;

    CinderWindowFlags flags;

} CinderWindowDesc;

CinderWindowDesc CinderDefaultWindowDesc(void);

CinderStatus CinderCreateWindow(CinderWindowDesc winDesc);
void CinderDestroyWindow(void);

CinderSize CinderGetWindowSize(void);

#endif /* CINDER_WINDOW_H_ */