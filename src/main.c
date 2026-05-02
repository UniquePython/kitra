#include "cinder.h"

#include <stddef.h>

int main(void)
{
    CinderInit(CINDER_SUBSYSTEM_VIDEO);
    CinderCreateWindow(NULL);

    while (CinderIsRunning())
    {
        CinderBeginFrame();
        CinderClearBackground(CINDER_RED);
        CinderEndFrame();
    }

    CinderQuit();
}