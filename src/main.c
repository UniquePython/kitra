#include "cinder.h"

int main(void)
{
    if (CinderInit(CINDER_SUBSYSTEM_VIDEO | CINDER_SUBSYSTEM_EVENTS) != CINDER_STATUS_OK)
        return -1;

    if (CinderCreateWindow(NULL) != CINDER_STATUS_OK)
        return -1;

    while (CinderIsRunning())
    {
        CinderBeginFrame();

        CinderClearBackground(CINDER_BLACK);

        // ---------------- LINE ----------------
        CinderDrawLine(50, 50, 250, 50, CINDER_WHITE);

        // ---------------- CIRCLE ----------------
        CinderDrawCircle(150, 150, 40, CINDER_RED);
        CinderDrawCircleOutline(300, 150, 40, CINDER_WHITE);

        // ---------------- TRIANGLE ----------------
        CinderDrawTriangle(50, 250, 150, 200, 250, 250, CINDER_GREEN);
        CinderDrawTriangleOutline(300, 250, 400, 200, 500, 250, CINDER_WHITE);

        // ---------------- RECTANGLE ----------------
        CinderRect rect1 = {50, 300, 100, 60};
        CinderRect rect2 = {200, 300, 100, 60};

        CinderDrawRect(rect1, CINDER_BLUE);
        CinderDrawRectOutline(rect2, CINDER_WHITE);

        // ---------------- ROUNDED RECTANGLE ----------------
        CinderRect rect3 = {350, 300, 120, 60};
        CinderRect rect4 = {500, 300, 120, 60};

        CinderDrawRoundedRect(rect3, 15, CINDER_YELLOW);
        CinderDrawRoundedRectOutline(rect4, 15, CINDER_WHITE);

        CinderEndFrame();
    }

    CinderQuit();
    return 0;
}