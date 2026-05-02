#include "cinder.h"

#include <stddef.h>

int main(void)
{
    CinderInit(CINDER_SUBSYSTEM_VIDEO);
    CinderCreateWindow(NULL);

    while (CinderIsRunning())
    {
        CinderBeginFrame();
        CinderClearBackground(CINDER_BLACK);

        CinderDrawRect((CinderRect){50, 50, 50, 100}, CINDER_RED);

        CinderEndFrame();
    }

    CinderQuit();
}