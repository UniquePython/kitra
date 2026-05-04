#include "cinder.h"

#include <math.h>
#include <stdio.h>

/* =============================================================================
 * Configuration
 * =============================================================================
 *
 * Point FONT_PATH at any TrueType font (.ttf) on your system, for example:
 *
 *   Linux:   "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"
 *   macOS:   "/System/Library/Fonts/Helvetica.ttc"
 *   Windows: "C:/Windows/Fonts/arial.ttf"
 */
#define FONT_PATH "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"

#define FONT_SIZE_SCORE 48
#define FONT_SIZE_MSG 24

#define WINDOW_W 800
#define WINDOW_H 600

#define PADDLE_W 12
#define PADDLE_H 80
#define PADDLE_SPEED 400.0f
#define PADDLE_MARGIN 40

#define BALL_SIZE 12
#define BALL_BASE_SPEED 300.0f
#define BALL_SPEED_INC 25.0f      /* px/s added to ball speed per paddle hit  */
#define BALL_MAX_BOUNCE_DEG 60.0f /* maximum bounce angle off a paddle center */

#define WIN_SCORE 5
#define SCORE_PAUSE 1.0f /* seconds to freeze after a point is scored */

/* =============================================================================
 * Types
 * ============================================================================= */

typedef enum
{
    STATE_PLAYING,
    STATE_SCORED, /* brief freeze between rally end and next serve */
    STATE_OVER,
} GameState;

/* Plain float rect — avoids repeated int casts in game logic. */
typedef struct
{
    float x, y, w, h;
} Rect;

typedef struct
{
    Rect rect;
    float speed;
} Paddle;

typedef struct
{
    float x, y;   /* top-left corner          */
    float vx, vy; /* velocity (px/s)          */
    float speed;  /* scalar speed, grows each paddle hit, resets on score */
} Ball;

typedef struct
{
    Paddle left;
    Paddle right;
    Ball ball;
    int scoreLeft;
    int scoreRight;
    GameState state;
    CinderTimer pauseTimer;
    int winner;        /*  0 = left, 1 = right, -1 = none yet */
    int serveDir;      /* +1 = toward right, -1 = toward left */
    int serveAngleIdx; /* cycles through kServeAngles[]        */
} Game;

/* =============================================================================
 * Constants
 * ============================================================================= */

static const float kPi = 3.14159265358979f;

/*
 * Preset serve angles (degrees from horizontal).
 * Fixed values keep the game deterministic — good for example code — while
 * still providing variety across rallies.
 */
static const float kServeAngles[] = {20.0f, -20.0f, 30.0f, -30.0f, 15.0f, -15.0f};
#define SERVE_ANGLE_COUNT ((int)(sizeof(kServeAngles) / sizeof(kServeAngles[0])))

/* =============================================================================
 * Helpers
 * ============================================================================= */

static float DegToRad(float deg)
{
    return deg * kPi / 180.0f;
}

static float Clampf(float v, float lo, float hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

static bool RectsOverlap(Rect a, Rect b)
{
    return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y;
}

static CinderRect ToCinderRect(Rect r)
{
    return (CinderRect){.x = (int)r.x, .y = (int)r.y, .w = (int)r.w, .h = (int)r.h};
}

/* =============================================================================
 * Game logic
 * ============================================================================= */

static void ServeBall(Game *g)
{
    g->ball.x = (WINDOW_W - BALL_SIZE) * 0.5f;
    g->ball.y = (WINDOW_H - BALL_SIZE) * 0.5f;
    g->ball.speed = BALL_BASE_SPEED;

    float angle = DegToRad(kServeAngles[g->serveAngleIdx]);
    g->serveAngleIdx = (g->serveAngleIdx + 1) % SERVE_ANGLE_COUNT;

    g->ball.vx = (float)g->serveDir * g->ball.speed * cosf(angle);
    g->ball.vy = g->ball.speed * sinf(angle);
}

static void ResetGame(Game *g)
{
    g->left.rect = (Rect){PADDLE_MARGIN,
                          (WINDOW_H - PADDLE_H) * 0.5f,
                          PADDLE_W, PADDLE_H};
    g->left.speed = PADDLE_SPEED;

    g->right.rect = (Rect){WINDOW_W - PADDLE_MARGIN - PADDLE_W,
                           (WINDOW_H - PADDLE_H) * 0.5f,
                           PADDLE_W, PADDLE_H};
    g->right.speed = PADDLE_SPEED;

    g->scoreLeft = 0;
    g->scoreRight = 0;
    g->state = STATE_PLAYING;
    g->winner = -1;
    g->serveDir = 1;
    g->serveAngleIdx = 0;

    ServeBall(g);
}

static void MovePaddle(Paddle *p, float dy)
{
    p->rect.y = Clampf(p->rect.y + dy, 0.0f, (float)(WINDOW_H - PADDLE_H));
}

/*
 * Reflect the ball off a paddle and apply the angle-from-hit-position rule:
 *   - Top third of paddle  → steeper upward angle
 *   - Middle of paddle     → nearly flat
 *   - Bottom third         → steeper downward angle
 *
 * dirX must be +1.0f (ball came off left paddle) or -1.0f (right paddle).
 */
static void ResolvePaddleCollision(Ball *b, const Paddle *p, float dirX)
{
    float ballCenterY = b->y + BALL_SIZE * 0.5f;
    float paddleCenterY = p->rect.y + p->rect.h * 0.5f;
    float hitFactor = Clampf(
        (ballCenterY - paddleCenterY) / (p->rect.h * 0.5f), -1.0f, 1.0f);

    float bounceAngle = hitFactor * DegToRad(BALL_MAX_BOUNCE_DEG);

    b->speed += BALL_SPEED_INC;
    b->vx = dirX * b->speed * cosf(bounceAngle);
    b->vy = b->speed * sinf(bounceAngle);
}

static void UpdatePlaying(Game *g, float dt)
{
    /* --- Paddle input ---------------------------------------------------- */

    if (CinderIsKeyDown(CINDER_KEY_W))
        MovePaddle(&g->left, -g->left.speed * dt);
    if (CinderIsKeyDown(CINDER_KEY_S))
        MovePaddle(&g->left, g->left.speed * dt);
    if (CinderIsKeyDown(CINDER_KEY_UP))
        MovePaddle(&g->right, -g->right.speed * dt);
    if (CinderIsKeyDown(CINDER_KEY_DOWN))
        MovePaddle(&g->right, g->right.speed * dt);

    /* --- Ball movement --------------------------------------------------- */

    g->ball.x += g->ball.vx * dt;
    g->ball.y += g->ball.vy * dt;

    /* --- Top / bottom wall bounce --------------------------------------- */

    if (g->ball.y <= 0.0f)
    {
        g->ball.y = 0.0f;
        g->ball.vy = fabsf(g->ball.vy);
    }
    else if (g->ball.y + BALL_SIZE >= (float)WINDOW_H)
    {
        g->ball.y = (float)(WINDOW_H - BALL_SIZE);
        g->ball.vy = -fabsf(g->ball.vy);
    }

    /* --- Paddle collisions ----------------------------------------------- *
     * Only test the paddle the ball is currently moving toward, which avoids
     * a double-hit when the ball is slow and overlaps for multiple frames.   */

    Rect ballRect = {g->ball.x, g->ball.y, BALL_SIZE, BALL_SIZE};

    if (g->ball.vx < 0.0f && RectsOverlap(ballRect, g->left.rect))
    {
        g->ball.x = g->left.rect.x + g->left.rect.w; /* push out of paddle */
        ResolvePaddleCollision(&g->ball, &g->left, +1.0f);
    }
    else if (g->ball.vx > 0.0f && RectsOverlap(ballRect, g->right.rect))
    {
        g->ball.x = g->right.rect.x - (float)BALL_SIZE;
        ResolvePaddleCollision(&g->ball, &g->right, -1.0f);
    }

    /* --- Scoring --------------------------------------------------------- *
     * After a score, the ball is served toward the player who just lost so
     * that they get first contact at the start of the next rally.            */

    if (g->ball.x + BALL_SIZE < 0.0f)
    {
        g->scoreRight++;
        g->serveDir = -1; /* serve left — left player just lost */
    }
    else if (g->ball.x > (float)WINDOW_W)
    {
        g->scoreLeft++;
        g->serveDir = 1; /* serve right — right player just lost */
    }
    else
    {
        return; /* no score this frame */
    }

    if (g->scoreLeft >= WIN_SCORE)
    {
        g->state = STATE_OVER;
        g->winner = 0;
    }
    else if (g->scoreRight >= WIN_SCORE)
    {
        g->state = STATE_OVER;
        g->winner = 1;
    }
    else
    {
        g->state = STATE_SCORED;
        g->pauseTimer = CinderStartTimer();
    }
}

static void UpdateScored(Game *g)
{
    if (CinderTimerDone(&g->pauseTimer, SCORE_PAUSE))
    {
        g->state = STATE_PLAYING;
        ServeBall(g);
    }
}

static void UpdateOver(Game *g)
{
    if (CinderIsKeyPressed(CINDER_KEY_ENTER))
        ResetGame(g);
}

/* =============================================================================
 * Rendering
 * ============================================================================= */

static void DrawCenteredText(CinderFont *font, const char *text, int cx, int y)
{
    CinderSize sz = CinderMeasureText(font, text);
    CinderDrawText(font, text, cx - sz.w / 2, y, CINDER_WHITE);
}

static void DrawScore(CinderFont *font, int scoreLeft, int scoreRight)
{
    char buf[8];

    snprintf(buf, sizeof(buf), "%d", scoreLeft);
    DrawCenteredText(font, buf, WINDOW_W / 4, 24);

    snprintf(buf, sizeof(buf), "%d", scoreRight);
    DrawCenteredText(font, buf, WINDOW_W * 3 / 4, 24);
}

static void DrawGame(const Game *g, CinderFont *scoreFont, CinderFont *msgFont)
{
    CinderClearBackground(CINDER_BLACK);

    CinderDrawRect(ToCinderRect(g->left.rect), CINDER_WHITE);
    CinderDrawRect(ToCinderRect(g->right.rect), CINDER_WHITE);

    if (g->state != STATE_OVER)
    {
        CinderRect ballRect = {.x = (int)g->ball.x, .y = (int)g->ball.y, .w = BALL_SIZE, .h = BALL_SIZE};
        CinderDrawRect(ballRect, CINDER_WHITE);
    }

    DrawScore(scoreFont, g->scoreLeft, g->scoreRight);

    if (g->state == STATE_OVER)
    {
        const char *msg = (g->winner == 0) ? "Left player wins!"
                                           : "Right player wins!";
        DrawCenteredText(scoreFont, msg, WINDOW_W / 2, WINDOW_H / 2 - 40);
        DrawCenteredText(msgFont, "Press Enter to play again", WINDOW_W / 2, WINDOW_H / 2 + 16);
    }
}

/* =============================================================================
 * Entry point
 * ============================================================================= */

int main(void)
{
    if (CinderInit(CINDER_SUBSYSTEM_ALL) != CINDER_STATUS_OK)
        return 1;

    CinderWindowDesc winDesc = CinderDefaultWindowDesc();
    winDesc.title = "Pong";
    winDesc.size.w = WINDOW_W;
    winDesc.size.h = WINDOW_H;

    if (CinderCreateWindow(winDesc) != CINDER_STATUS_OK)
        return 1;

    CinderFont *scoreFont = CinderLoadFont(FONT_PATH, FONT_SIZE_SCORE);
    CinderFont *msgFont = CinderLoadFont(FONT_PATH, FONT_SIZE_MSG);

    if (!scoreFont || !msgFont)
    {
        CinderDestroyFont(&scoreFont);
        CinderDestroyFont(&msgFont);
        CinderQuit();
        return 1;
    }

    CinderSetTargetFPS(60);

    Game g = {0};
    ResetGame(&g);

    while (CinderIsRunning())
    {
        CinderBeginFrame();

        if (CinderIsKeyPressed(CINDER_KEY_ESCAPE))
            CinderRequestQuit();

        float dt = CinderGetDeltaTime();

        switch (g.state)
        {
        case STATE_PLAYING:
            UpdatePlaying(&g, dt);
            break;
        case STATE_SCORED:
            UpdateScored(&g);
            break;
        case STATE_OVER:
            UpdateOver(&g);
            break;
        }

        DrawGame(&g, scoreFont, msgFont);

        CinderEndFrame();
    }

    CinderDestroyFont(&scoreFont);
    CinderDestroyFont(&msgFont);
    CinderQuit();
    return 0;
}