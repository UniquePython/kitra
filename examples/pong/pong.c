/*
 * pong.c — Full Pong using the Cinder library.
 *
 * Controls
 *   Left  player : W / S
 *   Right player : UP / DOWN
 *   Quit         : ESC
 *   Restart      : SPACE or ENTER  (after a player wins)
 *
 */

#include "cinder.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* =========================================================================
   Constants
   ========================================================================= */

#define WWIDTH 800
#define WHEIGHT 600

#define PADDLE_W 14
#define PADDLE_H 90
#define PADDLE_SPEED 420.0f
#define PADDLE_MARGIN 30

#define BALL_SIZE 14
#define BALL_SPEED_INIT 280.0f
#define BALL_SPEED_INC 15.0f
#define BALL_MAX_SPEED 700.0f

#define WIN_SCORE 7

#define FONT_BOLD "/home/maharaja/.local/share/fonts/FiraCodeNF/FiraCodeNerdFont-Bold.ttf"
#define FONT_REGULAR "/home/maharaja/.local/share/fonts/FiraCodeNF/FiraCodeNerdFont-Regular.ttf"

/* =========================================================================
   Game data
   ========================================================================= */

typedef struct
{
    float x, y, vx, vy, speed;
} Ball;
typedef struct
{
    float x, y;
    int score;
} Paddle;
typedef enum
{
    ST_PLAYING,
    ST_PAUSE,
    ST_WIN
} GameState;

static Ball g_ball;
static Paddle g_lp, g_rp;
static GameState g_state;
static float g_pauseTimer;
static int g_winner;  /* 0 = left, 1 = right */
static float g_flash; /* accumulator for blinking text */

/* Fonts */
static CinderFont *g_fontScore = NULL; /* Bold 72pt  — live scores   */
static CinderFont *g_fontTitle = NULL; /* Bold 36pt  — win banner    */
static CinderFont *g_fontUI = NULL;    /* Regular 20pt — hints/prompts */

/* =========================================================================
   Centred text helper
   FiraCode is monospaced. Advance width ≈ point_size × 0.600 per glyph.
   ========================================================================= */

static void TxtC(CinderFont *font, int ptsize, const char *s, int y, CinderColor c)
{
    int estimated_w = (int)(strlen(s) * ptsize * 0.600f);
    int x = (WWIDTH - estimated_w) / 2;
    CinderDrawText(font, s, x, y, c);
}

/* =========================================================================
   Physics helpers
   ========================================================================= */

static void ResetBall(int dir)
{
    g_ball.x = (float)(WWIDTH / 2 - BALL_SIZE / 2);
    g_ball.y = (float)(WHEIGHT / 2 - BALL_SIZE / 2);
    g_ball.speed = BALL_SPEED_INIT;

    float angle = ((float)(rand() % 600) / 10.0f - 30.0f) * (3.14159265f / 180.0f);
    g_ball.vx = cosf(angle) * (float)dir * g_ball.speed;
    g_ball.vy = sinf(angle) * g_ball.speed;
}

static void NewGame(void)
{
    g_lp = (Paddle){(float)PADDLE_MARGIN,
                    (float)(WHEIGHT / 2 - PADDLE_H / 2), 0};
    g_rp = (Paddle){(float)(WWIDTH - PADDLE_MARGIN - PADDLE_W),
                    (float)(WHEIGHT / 2 - PADDLE_H / 2), 0};
    ResetBall((rand() % 2) ? 1 : -1);
    g_state = ST_PLAYING;
    g_pauseTimer = 0.0f;
    g_flash = 0.0f;
}

static void BounceOffPaddle(const Paddle *p, int dir)
{
    float rel = ((g_ball.y + BALL_SIZE * 0.5f) - (p->y + PADDLE_H * 0.5f)) / (PADDLE_H * 0.5f);
    if (rel < -1.0f)
        rel = -1.0f;
    if (rel > 1.0f)
        rel = 1.0f;

    g_ball.speed += BALL_SPEED_INC;
    if (g_ball.speed > BALL_MAX_SPEED)
        g_ball.speed = BALL_MAX_SPEED;

    float angle = rel * (3.14159265f / 3.0f); /* ±60° max */
    g_ball.vx = (float)dir * cosf(angle) * g_ball.speed;
    g_ball.vy = sinf(angle) * g_ball.speed;
}

/* =========================================================================
   Entry point
   ========================================================================= */

int main(void)
{
    srand((unsigned)time(NULL));

    if (CinderInit(CINDER_SUBSYSTEM_ALL) != CINDER_STATUS_OK)
        return 1;

    CinderWindowDesc wd = {
        .title = "PONG",
        .width = WWIDTH,
        .height = WHEIGHT,
        .x = CINDER_WINDOW_POS_CENTERED,
        .y = CINDER_WINDOW_POS_CENTERED,
        .flags = CINDER_WINDOW_SHOWN,
    };
    if (CinderCreateWindow(&wd) != CINDER_STATUS_OK)
    {
        CinderQuit();
        return 1;
    }

    g_fontScore = CinderLoadFont(FONT_BOLD, 72);
    g_fontTitle = CinderLoadFont(FONT_BOLD, 36);
    g_fontUI = CinderLoadFont(FONT_REGULAR, 20);

    if (!g_fontScore || !g_fontTitle || !g_fontUI)
    {
        fprintf(stderr, "Failed to load fonts: %s\n", CinderGetError());
        CinderDestroyFont(&g_fontScore);
        CinderDestroyFont(&g_fontTitle);
        CinderDestroyFont(&g_fontUI);
        CinderDestroyWindow();
        CinderQuit();
        return 1;
    }

    CinderSetTargetFPS(60);
    NewGame();

    /* Palette */
    const CinderColor COL_BG = CINDER_COLOR_RGB(15, 15, 25);
    const CinderColor COL_DIV = CINDER_COLOR_RGB(48, 48, 64);
    const CinderColor COL_PADDLE = CINDER_COLOR_RGB(235, 235, 255);
    const CinderColor COL_BALL = CINDER_COLOR_RGB(255, 255, 255);
    const CinderColor COL_SCORE = CINDER_COLOR_RGB(155, 155, 190);
    const CinderColor COL_HINT = CINDER_COLOR_RGB(68, 68, 86);
    const CinderColor COL_YELLOW = CINDER_COLOR_RGB(255, 218, 40);
    const CinderColor COL_WHITE = CINDER_COLOR_RGB(255, 255, 255);
    const CinderColor COL_OVERLAY = CINDER_COLOR_RGBA(10, 10, 28, 220);

    while (CinderIsRunning())
    {
        CinderBeginFrame();

        float dt = CinderGetDeltaTime();
        if (dt > 0.05f)
            dt = 0.05f;

        if (CinderIsKeyPressed(CINDER_KEY_ESCAPE))
            CinderRequestQuit();
        g_flash += dt;

        /* ==============================================================
           UPDATE
           ============================================================== */

        if (g_state == ST_PLAYING)
        {

            /* --- Paddles --- */
            if (CinderIsKeyDown(CINDER_KEY_W))
                g_lp.y -= PADDLE_SPEED * dt;
            if (CinderIsKeyDown(CINDER_KEY_S))
                g_lp.y += PADDLE_SPEED * dt;
            if (CinderIsKeyDown(CINDER_KEY_UP))
                g_rp.y -= PADDLE_SPEED * dt;
            if (CinderIsKeyDown(CINDER_KEY_DOWN))
                g_rp.y += PADDLE_SPEED * dt;

            if (g_lp.y < 0)
                g_lp.y = 0.0f;
            if (g_lp.y + PADDLE_H > WHEIGHT)
                g_lp.y = (float)(WHEIGHT - PADDLE_H);
            if (g_rp.y < 0)
                g_rp.y = 0.0f;
            if (g_rp.y + PADDLE_H > WHEIGHT)
                g_rp.y = (float)(WHEIGHT - PADDLE_H);

            /* --- Ball --- */
            g_ball.x += g_ball.vx * dt;
            g_ball.y += g_ball.vy * dt;

            /* Top / bottom walls */
            if (g_ball.y < 0)
            {
                g_ball.y = 0.0f;
                g_ball.vy = fabsf(g_ball.vy);
            }
            if (g_ball.y + BALL_SIZE > WHEIGHT)
            {
                g_ball.y = (float)(WHEIGHT - BALL_SIZE);
                g_ball.vy = -fabsf(g_ball.vy);
            }

            /* Left paddle collision */
            if (g_ball.vx < 0 && g_ball.x <= g_lp.x + PADDLE_W && g_ball.x + BALL_SIZE >= g_lp.x && g_ball.y + BALL_SIZE > g_lp.y && g_ball.y < g_lp.y + PADDLE_H)
            {
                g_ball.x = g_lp.x + PADDLE_W;
                BounceOffPaddle(&g_lp, +1);
            }

            /* Right paddle collision */
            if (g_ball.vx > 0 && g_ball.x + BALL_SIZE >= g_rp.x && g_ball.x <= g_rp.x + PADDLE_W && g_ball.y + BALL_SIZE > g_rp.y && g_ball.y < g_rp.y + PADDLE_H)
            {
                g_ball.x = g_rp.x - BALL_SIZE;
                BounceOffPaddle(&g_rp, -1);
            }

            /* Scoring */
            if (g_ball.x + BALL_SIZE < 0)
            {
                g_rp.score++;
                if (g_rp.score >= WIN_SCORE)
                {
                    g_winner = 1;
                    g_state = ST_WIN;
                }
                else
                {
                    g_state = ST_PAUSE;
                    g_pauseTimer = 1.4f;
                }
            }
            if (g_ball.x > WWIDTH)
            {
                g_lp.score++;
                if (g_lp.score >= WIN_SCORE)
                {
                    g_winner = 0;
                    g_state = ST_WIN;
                }
                else
                {
                    g_state = ST_PAUSE;
                    g_pauseTimer = 1.4f;
                }
            }
        }
        else if (g_state == ST_PAUSE)
        {
            g_pauseTimer -= dt;
            if (g_pauseTimer <= 0.0f)
            {
                ResetBall((rand() % 2) ? 1 : -1);
                g_state = ST_PLAYING;
            }
        }
        else
        { /* ST_WIN */
            if (CinderIsKeyPressed(CINDER_KEY_SPACE) ||
                CinderIsKeyPressed(CINDER_KEY_ENTER))
                NewGame();
        }

        /* ==============================================================
           DRAW
           ============================================================== */

        CinderClearBackground(COL_BG);

        /* Centre dashes */
        for (int y = 10; y < WHEIGHT; y += 28)
        {
            CinderRect d = {WWIDTH / 2 - 2, y, 4, 14};
            CinderDrawRect(d, COL_DIV);
        }

        /* Paddles */
        CinderDrawRoundedRect(
            (CinderRect){(int)g_lp.x, (int)g_lp.y, PADDLE_W, PADDLE_H}, 4, COL_PADDLE);
        CinderDrawRoundedRect(
            (CinderRect){(int)g_rp.x, (int)g_rp.y, PADDLE_W, PADDLE_H}, 4, COL_PADDLE);

        /* Ball */
        if (g_state == ST_PLAYING)
        {
            CinderDrawRoundedRect(
                (CinderRect){(int)g_ball.x, (int)g_ball.y, BALL_SIZE, BALL_SIZE}, 3, COL_BALL);
        }

        /* Scores
           FiraCode-Bold 72pt: each digit ≈ 43px wide.
           Place score centred in each court half. */
        {
            char ls[8], rs[8];
            snprintf(ls, sizeof ls, "%d", g_lp.score);
            snprintf(rs, sizeof rs, "%d", g_rp.score);

            int digit_w = (int)(1 * 72 * 0.600f); /* single digit estimate */
            int lx = WWIDTH / 4 - digit_w / 2;
            int rx = 3 * WWIDTH / 4 - digit_w / 2;

            CinderDrawText(g_fontScore, ls, lx, 18, COL_SCORE);
            CinderDrawText(g_fontScore, rs, rx, 18, COL_SCORE);
        }

        /* "POINT!" flash during the first ~0.8s of the pause */
        if (g_state == ST_PAUSE && g_pauseTimer > 0.6f)
        {
            TxtC(g_fontTitle, 36, "POINT!", WHEIGHT / 2 - 24, COL_YELLOW);
        }

        /* Win overlay */
        if (g_state == ST_WIN)
        {
            CinderRect box = {WWIDTH / 2 - 200, WHEIGHT / 2 - 82, 400, 164};
            CinderDrawRoundedRect(box, 16, COL_OVERLAY);
            CinderDrawRoundedRectOutline(box, 16, COL_WHITE);

            char msg[20];
            snprintf(msg, sizeof msg, "P%d WINS!", g_winner + 1);
            TxtC(g_fontTitle, 36, msg, WHEIGHT / 2 - 62, COL_YELLOW);

            if ((int)(g_flash * 2.0f) % 2 == 0)
                TxtC(g_fontUI, 20, "Press ENTER to play again", WHEIGHT / 2 + 24, COL_WHITE);
        }

        /* Control hints */
        CinderDrawText(g_fontUI, "W / S", 20, WHEIGHT - 32, COL_HINT);
        CinderDrawText(g_fontUI, "UP / DOWN", WWIDTH - 132, WHEIGHT - 32, COL_HINT);

        CinderEndFrame();
    }

    CinderDestroyFont(&g_fontScore);
    CinderDestroyFont(&g_fontTitle);
    CinderDestroyFont(&g_fontUI);
    CinderDestroyWindow();
    CinderQuit();
    return 0;
}