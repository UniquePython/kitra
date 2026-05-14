#include "kitra.h"

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
#define FONT_SIZE_MSG 20

#define WINDOW_W 800
#define WINDOW_H 600

#define PADDLE_W 12
#define PADDLE_H 80
#define PADDLE_SPEED 400.0f
#define PADDLE_MARGIN 40
#define PADDLE_RADIUS 4 /* corner radius for KitraDrawRoundedRect */

#define BALL_RADIUS 8
#define BALL_GLOW_RADIUS 16 /* radius of the soft glow ring behind the ball */
#define BALL_BASE_SPEED 300.0f
#define BALL_SPEED_INC 25.0f
#define BALL_MAX_BOUNCE_DEG 60.0f

#define WIN_SCORE 5
#define SCORE_PAUSE 1.0f /* seconds to freeze after a point */

#define DASH_W 4
#define DASH_H 20
#define DASH_GAP 12

/* =============================================================================
 * Types
 * ============================================================================= */

typedef enum
{
    STATE_PLAYING,
    STATE_SCORED,
    STATE_OVER,
} GameState;

typedef struct
{
    float x, y; /* top-left corner; width/height are compile-time constants */
    float speed;
} Paddle;

typedef struct
{
    float x, y; /* center of the ball */
    float vx, vy;
    float speed;
} Ball;

typedef struct
{
    Paddle left;
    Paddle right;
    Ball ball;
    int scoreLeft;
    int scoreRight;
    GameState state;
    KitraTimer pauseTimer;
    int winner;   /*  0 = left, 1 = right, -1 = none yet */
    int serveDir; /* +1 = toward right, -1 = toward left */
    int serveAngleIdx;
} Game;

/* =============================================================================
 * Constants
 * ============================================================================= */

/*
 * Preset serve angles (degrees from horizontal).
 * Fixed values keep the game deterministic while still varying across rallies.
 */
static const float kServeAngles[] = {20.f, -20.f, 30.f, -30.f, 15.f, -15.f};
#define SERVE_ANGLE_COUNT ((int)(sizeof(kServeAngles) / sizeof(kServeAngles[0])))

static const KitraColor kBgColor = {15, 15, 25, 255};
static const KitraColor kDashColor = {255, 255, 255, 50};
static const KitraColor kGlowColor = {255, 255, 255, 55};
static const KitraColor kSubtextColor = {160, 160, 160, 255};

/* =============================================================================
 * Helpers
 * ============================================================================= */

static float Clampf(float v, float lo, float hi)
{
    return v < lo ? lo : v > hi ? hi
                                : v;
}

/* Returns the KitraRect for a paddle (used for both drawing and collision). */
static KitraRect PaddleRect(const Paddle *p)
{
    return (KitraRect){(int)p->x, (int)p->y, PADDLE_W, PADDLE_H};
}

/*
 * Axis-aligned bounding box of the ball.
 * Used for paddle collision; the ball is drawn as a circle but tested as a
 * square because it is cheaper and barely noticeable at this size.
 */
static KitraRect BallAABB(const Ball *b)
{
    return (KitraRect){
        (int)(b->x - BALL_RADIUS), (int)(b->y - BALL_RADIUS),
        BALL_RADIUS * 2, BALL_RADIUS * 2};
}

static void DrawCenteredText(KitraFont *font, const char *text,
                             int cx, int y, KitraColor color)
{
    KitraSize sz = KitraMeasureText(font, text);
    KitraDrawText(font, text, cx - sz.w / 2, y, color);
}

/* =============================================================================
 * Game logic
 * ============================================================================= */

static void ServeBall(Game *g)
{
    g->ball.x = WINDOW_W * 0.5f;
    g->ball.y = WINDOW_H * 0.5f;
    g->ball.speed = BALL_BASE_SPEED;

    float angle = (float)KITRA_DEG2RAD(kServeAngles[g->serveAngleIdx]);
    g->serveAngleIdx = (g->serveAngleIdx + 1) % SERVE_ANGLE_COUNT;

    g->ball.vx = (float)g->serveDir * g->ball.speed * cosf(angle);
    g->ball.vy = g->ball.speed * sinf(angle);
}

static void ResetGame(Game *g)
{
    g->left = (Paddle){PADDLE_MARGIN,
                       (WINDOW_H - PADDLE_H) * 0.5f,
                       PADDLE_SPEED};
    g->right = (Paddle){WINDOW_W - PADDLE_MARGIN - PADDLE_W,
                        (WINDOW_H - PADDLE_H) * 0.5f,
                        PADDLE_SPEED};

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
    p->y = Clampf(p->y + dy, 0.0f, (float)(WINDOW_H - PADDLE_H));
}

/*
 * Reflect the ball off a paddle and apply the angle-from-hit-position rule.
 * dirX must be +1.0f (left paddle) or -1.0f (right paddle).
 */
static void ResolvePaddleCollision(Ball *b, const Paddle *p, float dirX)
{
    float paddleCenterY = p->y + PADDLE_H * 0.5f;
    float hitFactor = Clampf(
        (b->y - paddleCenterY) / (PADDLE_H * 0.5f), -1.0f, 1.0f);

    float bounceAngle = hitFactor * (float)KITRA_DEG2RAD(BALL_MAX_BOUNCE_DEG);

    b->speed += BALL_SPEED_INC;
    b->vx = dirX * b->speed * cosf(bounceAngle);
    b->vy = b->speed * sinf(bounceAngle);
}

static void UpdatePlaying(Game *g, float dt)
{
    /* --- Paddle input ---------------------------------------------------- */

    if (KitraIsKeyDown(KITRA_KEY_W))
        MovePaddle(&g->left, -g->left.speed * dt);
    if (KitraIsKeyDown(KITRA_KEY_S))
        MovePaddle(&g->left, g->left.speed * dt);
    if (KitraIsKeyDown(KITRA_KEY_UP))
        MovePaddle(&g->right, -g->right.speed * dt);
    if (KitraIsKeyDown(KITRA_KEY_DOWN))
        MovePaddle(&g->right, g->right.speed * dt);

    /* --- Ball movement --------------------------------------------------- */

    g->ball.x += g->ball.vx * dt;
    g->ball.y += g->ball.vy * dt;

    /* --- Top / bottom wall bounce --------------------------------------- */

    if (g->ball.y - BALL_RADIUS < 0.0f)
    {
        g->ball.y = BALL_RADIUS;
        g->ball.vy = fabsf(g->ball.vy);
    }
    else if (g->ball.y + BALL_RADIUS > (float)WINDOW_H)
    {
        g->ball.y = (float)WINDOW_H - BALL_RADIUS;
        g->ball.vy = -fabsf(g->ball.vy);
    }

    /* --- Paddle collisions ----------------------------------------------- *
     * Only test the paddle the ball is currently moving toward, which avoids
     * a double-hit when the ball is slow and overlaps for multiple frames.   */

    KitraRect ballBox = BallAABB(&g->ball);

    if (g->ball.vx < 0.0f && KitraRectsOverlap(ballBox, PaddleRect(&g->left)))
    {
        g->ball.x = g->left.x + PADDLE_W + BALL_RADIUS;
        ResolvePaddleCollision(&g->ball, &g->left, +1.0f);
    }
    else if (g->ball.vx > 0.0f && KitraRectsOverlap(ballBox, PaddleRect(&g->right)))
    {
        g->ball.x = g->right.x - BALL_RADIUS;
        ResolvePaddleCollision(&g->ball, &g->right, -1.0f);
    }

    /* --- Scoring --------------------------------------------------------- *
     * After a score, serve toward the player who just lost.                  */

    if (g->ball.x + BALL_RADIUS < 0.0f)
    {
        g->scoreRight++;
        g->serveDir = -1;
    }
    else if (g->ball.x - BALL_RADIUS > (float)WINDOW_W)
    {
        g->scoreLeft++;
        g->serveDir = 1;
    }
    else
    {
        return;
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
        g->pauseTimer = KitraStartTimer();
    }
}

static void UpdateScored(Game *g)
{
    if (KitraTimerDone(&g->pauseTimer, SCORE_PAUSE))
    {
        g->state = STATE_PLAYING;
        ServeBall(g);
    }
}

static void UpdateOver(Game *g)
{
    if (KitraIsKeyPressed(KITRA_KEY_ENTER))
        ResetGame(g);
}

/* =============================================================================
 * Rendering
 * ============================================================================= */

static void DrawCenterLine(void)
{
    int x = WINDOW_W / 2 - DASH_W / 2;
    for (int y = 0; y < WINDOW_H; y += DASH_H + DASH_GAP)
        KitraDrawRect((KitraRect){x, y, DASH_W, DASH_H}, kDashColor);
}

static void DrawGame(const Game *g, KitraFont *scoreFont, KitraFont *msgFont)
{
    KitraClearBackground(kBgColor);

    DrawCenterLine();

    /* Paddles */
    KitraDrawRoundedRect(PaddleRect(&g->left), PADDLE_RADIUS, KITRA_WHITE);
    KitraDrawRoundedRect(PaddleRect(&g->right), PADDLE_RADIUS, KITRA_WHITE);

    /* Ball: soft glow ring + solid core */
    if (g->state != STATE_OVER)
    {
        int bx = (int)g->ball.x;
        int by = (int)g->ball.y;
        KitraDrawCircle(bx, by, BALL_GLOW_RADIUS, kGlowColor);
        KitraDrawCircle(bx, by, BALL_RADIUS, KITRA_WHITE);
    }

    /* Scores */
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", g->scoreLeft);
    DrawCenteredText(scoreFont, buf, WINDOW_W / 4, 24, KITRA_WHITE);
    snprintf(buf, sizeof(buf), "%d", g->scoreRight);
    DrawCenteredText(scoreFont, buf, WINDOW_W * 3 / 4, 24, KITRA_WHITE);

    /* Control hints */
    if (g->state == STATE_PLAYING || g->state == STATE_SCORED)
    {
        DrawCenteredText(msgFont, "W / S", WINDOW_W / 4, WINDOW_H - 28, kSubtextColor);
        DrawCenteredText(msgFont, "UP / DOWN", WINDOW_W * 3 / 4, WINDOW_H - 28, kSubtextColor);
    }

    /* Game over overlay */
    if (g->state == STATE_OVER)
    {
        const char *msg = (g->winner == 0) ? "Left Player Wins!" : "Right Player Wins!";
        DrawCenteredText(scoreFont, msg, WINDOW_W / 2, WINDOW_H / 2 - 50, KITRA_WHITE);
        DrawCenteredText(msgFont, "Press Enter to play again",
                         WINDOW_W / 2, WINDOW_H / 2 + 20, kSubtextColor);
    }
}

/* =============================================================================
 * Entry point
 * ============================================================================= */

int main(void)
{
    if (KitraInit(KITRA_SUBSYSTEM_ALL) != KITRA_STATUS_OK)
        return 1;

    KitraWindowDesc winDesc = KitraDefaultWindowDesc();
    winDesc.title = "Pong";
    winDesc.size.w = WINDOW_W;
    winDesc.size.h = WINDOW_H;
    winDesc.centerX = true;
    winDesc.centerY = true;

    if (KitraCreateWindow(winDesc) != KITRA_STATUS_OK)
        return 1;

    KitraSetBlendMode(KITRA_BLEND_ALPHA);

    KitraFont *scoreFont = KitraLoadFont(FONT_PATH, FONT_SIZE_SCORE);
    KitraFont *msgFont = KitraLoadFont(FONT_PATH, FONT_SIZE_MSG);

    if (!scoreFont || !msgFont)
    {
        KitraDestroyFont(&scoreFont);
        KitraDestroyFont(&msgFont);
        KitraQuit();
        return 1;
    }

    KitraSetTargetFPS(60);

    Game g = {0};
    ResetGame(&g);

    while (KitraIsRunning())
    {
        KitraBeginFrame();

        if (KitraIsKeyPressed(KITRA_KEY_ESCAPE))
            KitraRequestQuit();

        float dt = KitraGetDeltaTime();

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

        KitraEndFrame();
    }

    KitraDestroyFont(&scoreFont);
    KitraDestroyFont(&msgFont);
    KitraQuit();
    return 0;
}