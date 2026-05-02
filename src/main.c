#include "cinder.h"

int main(void)
{
    if (CinderInit(CINDER_SUBSYSTEM_VIDEO | CINDER_SUBSYSTEM_EVENTS) != CINDER_STATUS_OK)
        return -1;

    if (CinderCreateWindow(NULL) != CINDER_STATUS_OK)
        return -1;

    CinderRect player = {100, 100, 50, 50};

    const float speed = 300.0f; // pixels per second

    float x = 100.0f;
    float y = 100.0f;

    while (CinderIsRunning())
    {
        CinderBeginFrame();

        float dt = CinderGetDeltaTime();

        if (CinderIsKeyDown(CINDER_KEY_W))
            y -= speed * dt;

        if (CinderIsKeyDown(CINDER_KEY_S))
            y += speed * dt;

        if (CinderIsKeyDown(CINDER_KEY_A))
            x -= speed * dt;

        if (CinderIsKeyDown(CINDER_KEY_D))
            x += speed * dt;

        player.x = (int)x;
        player.y = (int)y;

        CinderClearBackground(CINDER_BLACK);
        CinderDrawRect(player, CINDER_WHITE);

        CinderEndFrame();
    }

    CinderQuit();
    return 0;
}