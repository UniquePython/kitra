/*
 * snake.c — Snake built on the Cinder library.
 *
 * Controls
 *   Move         : Arrow keys  OR  W A S D
 *   Quit         : ESC
 *   Restart      : ENTER or SPACE  (after game over)
 *
 */

#include "cinder.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* =========================================================================
   Layout & tuning
   ========================================================================= */

#define WWIDTH 800
#define WHEIGHT 640

/* The playfield sits inside a border, leaving room for the HUD at the top. */
#define HUD_H 56 /* pixels reserved for score bar           */
#define BORDER 2 /* thickness of the playfield border       */
#define CELL 20  /* pixel size of one grid cell             */

/* Grid dimensions derived from the available space */
#define GRID_X ((WWIDTH) / CELL)          /* 40 columns  */
#define GRID_Y ((WHEIGHT - HUD_H) / CELL) /* 29 rows     */

/* Starting snake length (in cells, not counting the head) */
#define START_LEN 3

/* Move timer: seconds between steps.  Decreases as score rises. */
#define STEP_BASE 0.13f
#define STEP_MIN 0.045f
#define STEP_DEC 0.003f /* subtract per food eaten               */

#define MAX_CELLS (GRID_X * GRID_Y)

#define FONT_BOLD "/home/maharaja/.local/share/fonts/FiraCodeNF/FiraCodeNerdFont-Bold.ttf"
#define FONT_REGULAR "/home/maharaja/.local/share/fonts/FiraCodeNF/FiraCodeNerdFont-Regular.ttf"

/* =========================================================================
   Colours
   ========================================================================= */

static const CinderColor COL_BG = CINDER_COLOR_RGB(13, 17, 23);
static const CinderColor COL_BORDER = CINDER_COLOR_RGB(48, 56, 70);
static const CinderColor COL_GRID = CINDER_COLOR_RGB(20, 24, 32);
static const CinderColor COL_HUD_BG = CINDER_COLOR_RGB(18, 22, 30);
static const CinderColor COL_HUD_LINE = CINDER_COLOR_RGB(40, 48, 62);

static const CinderColor COL_SNAKE_HEAD = CINDER_COLOR_RGB(80, 220, 120);
static const CinderColor COL_SNAKE_BODY = CINDER_COLOR_RGB(42, 160, 80);
static const CinderColor COL_SNAKE_DARK = CINDER_COLOR_RGB(28, 110, 55);

static const CinderColor COL_FOOD = CINDER_COLOR_RGB(255, 80, 80);
static const CinderColor COL_FOOD_SHINE = CINDER_COLOR_RGB(255, 160, 160);

static const CinderColor COL_SCORE_LBL = CINDER_COLOR_RGB(100, 110, 130);
static const CinderColor COL_SCORE_VAL = CINDER_COLOR_RGB(230, 240, 255);
static const CinderColor COL_WHITE = CINDER_COLOR_RGB(255, 255, 255);
static const CinderColor COL_YELLOW = CINDER_COLOR_RGB(255, 218, 40);
static const CinderColor COL_OVERLAY = CINDER_COLOR_RGBA(8, 10, 18, 210);
static const CinderColor COL_DIMWHITE = CINDER_COLOR_RGB(170, 178, 200);

/* =========================================================================
   Game types
   ========================================================================= */

typedef struct
{
    int x, y;
} Cell;

typedef enum
{
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Dir;

typedef enum
{
    ST_TITLE,
    ST_PLAYING,
    ST_DEAD,
} GameState;

/* =========================================================================
   Global game state
   ========================================================================= */

static Cell g_snake[MAX_CELLS];
static int g_len;     /* number of cells occupied              */
static Dir g_dir;     /* current travel direction              */
static Dir g_nextDir; /* buffered input direction              */
static Cell g_food;

static int g_score;
static int g_hiScore;
static float g_stepInterval; /* seconds per move                      */
static float g_stepTimer;    /* time accumulated since last step      */
static float g_flash;        /* for blinking UI                       */
static float g_deathTimer;   /* small delay before showing game-over  */

static GameState g_state;

/* Fonts */
static CinderFont *g_fontBig = NULL; /* Bold 32pt  — "SNAKE", scores   */
static CinderFont *g_fontMed = NULL; /* Bold 22pt  — game over banner  */
static CinderFont *g_fontUI = NULL;  /* Regular 18pt — hints           */

/* =========================================================================
   Helpers
   ========================================================================= */

/* FiraCode monospaced estimate: advance ≈ pt × 0.600 */
static void TxtC(CinderFont *f, int pt, const char *s, int y, CinderColor c)
{
    int w = (int)(strlen(s) * pt * 0.600f);
    CinderDrawText(f, s, (WWIDTH - w) / 2, y, c);
}

/* Top-left corner of a grid cell in screen space */
static int CellPx(int g) { return g * CELL; }
static int CellPy(int g) { return HUD_H + g * CELL; }

/* Place food at a random empty cell */
static void SpawnFood(void)
{
    /* Mark occupied cells */
    static bool occ[GRID_X * GRID_Y];
    memset(occ, 0, sizeof occ);
    for (int i = 0; i < g_len; i++)
        occ[g_snake[i].y * GRID_X + g_snake[i].x] = true;

    /* Collect free cells */
    static int free_idx[GRID_X * GRID_Y];
    int fc = 0;
    for (int i = 0; i < GRID_X * GRID_Y; i++)
        if (!occ[i])
            free_idx[fc++] = i;

    if (fc == 0)
        return; /* shouldn't happen in a normal game */

    int pick = free_idx[rand() % fc];
    g_food.x = pick % GRID_X;
    g_food.y = pick / GRID_X;
}

static void NewGame(void)
{
    g_len = START_LEN + 1; /* +1 because index 0 is the head */

    /* Start in the middle, heading right */
    int sx = GRID_X / 2;
    int sy = GRID_Y / 2;
    for (int i = 0; i < g_len; i++)
    {
        g_snake[i].x = sx - i;
        g_snake[i].y = sy;
    }

    g_dir = DIR_RIGHT;
    g_nextDir = DIR_RIGHT;
    g_score = 0;
    g_stepInterval = STEP_BASE;
    g_stepTimer = 0.0f;
    g_flash = 0.0f;
    g_deathTimer = 0.0f;

    SpawnFood();
    g_state = ST_PLAYING;
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
        .title = "SNAKE",
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

    g_fontBig = CinderLoadFont(FONT_BOLD, 32);
    g_fontMed = CinderLoadFont(FONT_BOLD, 22);
    g_fontUI = CinderLoadFont(FONT_REGULAR, 18);

    if (!g_fontBig || !g_fontMed || !g_fontUI)
    {
        fprintf(stderr, "Font load failed: %s\n", CinderGetError());
        CinderDestroyFont(&g_fontBig);
        CinderDestroyFont(&g_fontMed);
        CinderDestroyFont(&g_fontUI);
        CinderDestroyWindow();
        CinderQuit();
        return 1;
    }

    g_hiScore = 0;
    g_state = ST_TITLE;
    g_flash = 0.0f;

    CinderSetTargetFPS(60);

    while (CinderIsRunning())
    {
        CinderBeginFrame();

        float dt = CinderGetDeltaTime();
        if (dt > 0.1f)
            dt = 0.1f;

        if (CinderIsKeyPressed(CINDER_KEY_ESCAPE))
            CinderRequestQuit();
        g_flash += dt;

        /* ==============================================================
           INPUT  (direction buffer — never allow 180° reversal)
           ============================================================== */

        if (g_state == ST_PLAYING)
        {
            if ((CinderIsKeyPressed(CINDER_KEY_UP) || CinderIsKeyPressed(CINDER_KEY_W)) && g_dir != DIR_DOWN)
                g_nextDir = DIR_UP;
            if ((CinderIsKeyPressed(CINDER_KEY_DOWN) || CinderIsKeyPressed(CINDER_KEY_S)) && g_dir != DIR_UP)
                g_nextDir = DIR_DOWN;
            if ((CinderIsKeyPressed(CINDER_KEY_LEFT) || CinderIsKeyPressed(CINDER_KEY_A)) && g_dir != DIR_RIGHT)
                g_nextDir = DIR_LEFT;
            if ((CinderIsKeyPressed(CINDER_KEY_RIGHT) || CinderIsKeyPressed(CINDER_KEY_D)) && g_dir != DIR_LEFT)
                g_nextDir = DIR_RIGHT;
        }

        if (g_state == ST_TITLE || g_state == ST_DEAD)
        {
            if (CinderIsKeyPressed(CINDER_KEY_ENTER) ||
                CinderIsKeyPressed(CINDER_KEY_SPACE))
                NewGame();
        }

        /* ==============================================================
           UPDATE
           ============================================================== */

        if (g_state == ST_PLAYING)
        {

            /* Death delay (let the player see the collision) */
            if (g_deathTimer > 0.0f)
            {
                g_deathTimer -= dt;
                if (g_deathTimer <= 0.0f)
                {
                    if (g_score > g_hiScore)
                        g_hiScore = g_score;
                    g_state = ST_DEAD;
                }
                goto draw; /* skip step logic during death flash */
            }

            g_stepTimer += dt;
            if (g_stepTimer >= g_stepInterval)
            {
                g_stepTimer -= g_stepInterval;

                /* Commit buffered direction */
                g_dir = g_nextDir;

                /* Compute new head position */
                Cell newHead = g_snake[0];
                switch (g_dir)
                {
                case DIR_UP:
                    newHead.y--;
                    break;
                case DIR_DOWN:
                    newHead.y++;
                    break;
                case DIR_LEFT:
                    newHead.x--;
                    break;
                case DIR_RIGHT:
                    newHead.x++;
                    break;
                }

                /* Wall collision */
                if (newHead.x < 0 || newHead.x >= GRID_X ||
                    newHead.y < 0 || newHead.y >= GRID_Y)
                {
                    g_deathTimer = 0.35f;
                    goto draw;
                }

                /* Self collision (skip tail tip — it will have moved) */
                for (int i = 0; i < g_len - 1; i++)
                {
                    if (g_snake[i].x == newHead.x && g_snake[i].y == newHead.y)
                    {
                        g_deathTimer = 0.35f;
                        goto draw;
                    }
                }

                /* Check food */
                bool ate = (newHead.x == g_food.x && newHead.y == g_food.y);

                /* Shift body: move from tail toward head */
                if (ate)
                {
                    /* Grow: push everything forward, no tail removal */
                    if (g_len < MAX_CELLS)
                    {
                        memmove(&g_snake[1], &g_snake[0], g_len * sizeof(Cell));
                        g_len++;
                    }
                }
                else
                {
                    memmove(&g_snake[1], &g_snake[0], (g_len - 1) * sizeof(Cell));
                }
                g_snake[0] = newHead;

                if (ate)
                {
                    g_score++;
                    g_stepInterval -= STEP_DEC;
                    if (g_stepInterval < STEP_MIN)
                        g_stepInterval = STEP_MIN;
                    SpawnFood();
                }
            }
        }

        /* ==============================================================
           DRAW
           ============================================================== */
    draw:;

        CinderClearBackground(COL_BG);

        /* --- HUD bar --- */
        CinderDrawRect((CinderRect){0, 0, WWIDTH, HUD_H}, COL_HUD_BG);
        CinderDrawLine(0, HUD_H, WWIDTH, HUD_H, COL_HUD_LINE);

        /* Title */
        CinderDrawText(g_fontBig, "SNAKE", 20, (HUD_H - 36) / 2, COL_SNAKE_HEAD);

        /* Score */
        {
            char buf[32];
            CinderDrawText(g_fontUI, "SCORE", WWIDTH / 2 - 100, 8, COL_SCORE_LBL);
            snprintf(buf, sizeof buf, "%d", g_score);
            CinderDrawText(g_fontBig, buf, WWIDTH / 2 - 100, 24, COL_SCORE_VAL);

            CinderDrawText(g_fontUI, "BEST", WWIDTH / 2 + 20, 8, COL_SCORE_LBL);
            snprintf(buf, sizeof buf, "%d", g_hiScore);
            CinderDrawText(g_fontBig, buf, WWIDTH / 2 + 20, 24, COL_SCORE_VAL);
        }

        /* --- Grid dots --- */
        for (int gx = 0; gx < GRID_X; gx++)
        {
            for (int gy = 0; gy < GRID_Y; gy++)
            {
                int px = CellPx(gx) + CELL / 2;
                int py = CellPy(gy) + CELL / 2;
                CinderDrawCircle(px, py, 1, COL_GRID);
            }
        }

        /* --- Playfield border --- */
        CinderDrawRectOutline(
            (CinderRect){0, HUD_H, WWIDTH, WHEIGHT - HUD_H}, COL_BORDER);

        /* --- Snake --- */
        if (g_state == ST_PLAYING || g_state == ST_DEAD)
        {

            /* Death flash: blink the whole snake red */
            bool dying = (g_deathTimer > 0.0f);
            bool blink = dying && ((int)(g_deathTimer * 12.0f) % 2 == 0);

            for (int i = g_len - 1; i >= 0; i--)
            {
                int px = CellPx(g_snake[i].x);
                int py = CellPy(g_snake[i].y);
                int pad = 2;

                CinderColor col;
                if (dying)
                {
                    col = blink ? CINDER_COLOR_RGB(255, 60, 60)
                                : CINDER_COLOR_RGB(160, 20, 20);
                }
                else if (i == 0)
                {
                    col = COL_SNAKE_HEAD;
                }
                else
                {
                    /* Fade body from bright to dark toward the tail */
                    float t = (float)i / (float)(g_len - 1);
                    uint8_t r = (uint8_t)(COL_SNAKE_BODY.r + t * (COL_SNAKE_DARK.r - COL_SNAKE_BODY.r));
                    uint8_t g = (uint8_t)(COL_SNAKE_BODY.g + t * (COL_SNAKE_DARK.g - COL_SNAKE_BODY.g));
                    uint8_t b = (uint8_t)(COL_SNAKE_BODY.b + t * (COL_SNAKE_DARK.b - COL_SNAKE_BODY.b));
                    col = CINDER_COLOR_RGB(r, g, b);
                }

                CinderDrawRoundedRect(
                    (CinderRect){px + pad, py + pad, CELL - pad * 2, CELL - pad * 2},
                    4, col);

                /* Eye on the head */
                if (i == 0 && !dying)
                {
                    int ex = px + CELL / 2;
                    int ey = py + CELL / 2;
                    int eo = 4; /* eye offset from centre */
                    switch (g_dir)
                    {
                    case DIR_RIGHT:
                        ex += eo;
                        break;
                    case DIR_LEFT:
                        ex -= eo;
                        break;
                    case DIR_UP:
                        ey -= eo;
                        break;
                    case DIR_DOWN:
                        ey += eo;
                        break;
                    }
                    CinderDrawCircle(ex, ey, 3, CINDER_COLOR_RGB(10, 10, 10));
                    CinderDrawCircle(ex + 1, ey - 1, 1, COL_WHITE);
                }
            }
        }

        /* --- Food --- */
        if (g_state == ST_PLAYING)
        {
            int fx = CellPx(g_food.x) + CELL / 2;
            int fy = CellPy(g_food.y) + CELL / 2;
            int r = CELL / 2 - 2;

            /* Subtle pulse */
            float pulse = 0.5f + 0.5f * sinf(g_flash * 5.0f);
            int pr = r + (int)(pulse * 2.0f);

            CinderDrawCircle(fx, fy, pr, COL_FOOD);
            CinderDrawCircle(fx - 2, fy - 2, 2, COL_FOOD_SHINE); /* shine dot */
        }

        /* --- Title screen overlay --- */
        if (g_state == ST_TITLE)
        {
            CinderRect ov = {60, HUD_H + 60, WWIDTH - 120, WHEIGHT - HUD_H - 120};
            CinderDrawRoundedRect(ov, 16, COL_OVERLAY);
            CinderDrawRoundedRectOutline(ov, 16, COL_SNAKE_HEAD);

            TxtC(g_fontBig, 32, "SNAKE", HUD_H + 90, COL_SNAKE_HEAD);
            TxtC(g_fontUI, 18, "Eat the food. Don't die.", HUD_H + 148, COL_DIMWHITE);
            TxtC(g_fontUI, 18, "Arrow keys or WASD to move", HUD_H + 176, COL_DIMWHITE);

            if ((int)(g_flash * 2.2f) % 2 == 0)
                TxtC(g_fontMed, 22, "Press ENTER to start", HUD_H + 240, COL_YELLOW);
        }

        /* --- Game over overlay --- */
        if (g_state == ST_DEAD)
        {
            CinderRect ov = {80, HUD_H + 80, WWIDTH - 160, WHEIGHT - HUD_H - 160};
            CinderDrawRoundedRect(ov, 16, COL_OVERLAY);
            CinderDrawRoundedRectOutline(ov, 16, CINDER_COLOR_RGB(200, 50, 50));

            TxtC(g_fontBig, 32, "GAME OVER", HUD_H + 110, CINDER_COLOR_RGB(255, 80, 80));

            char sbuf[32];
            snprintf(sbuf, sizeof sbuf, "Score: %d", g_score);
            TxtC(g_fontMed, 22, sbuf, HUD_H + 165, COL_WHITE);

            if (g_score == g_hiScore && g_score > 0)
            {
                TxtC(g_fontUI, 18, "New best!", HUD_H + 200, COL_YELLOW);
            }

            if ((int)(g_flash * 2.2f) % 2 == 0)
                TxtC(g_fontMed, 22, "Press ENTER to retry", HUD_H + 240, COL_YELLOW);
        }

        CinderEndFrame();
    }

    CinderDestroyFont(&g_fontBig);
    CinderDestroyFont(&g_fontMed);
    CinderDestroyFont(&g_fontUI);
    CinderDestroyWindow();
    CinderQuit();
    return 0;
}