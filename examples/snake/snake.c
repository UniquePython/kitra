#include "cinder.h"

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

#define FONT_SIZE_SCORE 36
#define FONT_SIZE_MSG 20

#define WINDOW_W 800
#define WINDOW_H 600

#define CELL_SIZE 20
#define GRID_W (WINDOW_W / CELL_SIZE) /* 40 */
#define GRID_H (WINDOW_H / CELL_SIZE) /* 30 */
#define MAX_SNAKE (GRID_W * GRID_H)   /* maximum possible snake length */

#define MOVE_INTERVAL_INIT 0.12f /* seconds between steps at game start   */
#define MOVE_INTERVAL_MIN 0.05f  /* fastest the snake can move            */
#define SPEED_STEP 0.004f        /* interval reduction per food eaten      */

/* Visual */
#define CELL_PAD 2     /* gap between adjacent cells                       */
#define SEG_RADIUS 3   /* corner radius for snake segment rounded-rects    */
#define FOOD_RADIUS 7  /* food circle radius (fits inside one cell)        */
#define FOOD_GLOW_R 13 /* radius of the soft glow drawn behind the food    */

/* =============================================================================
 * Types
 * ============================================================================= */

typedef enum
{
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

typedef struct
{
    int x, y;
} Cell;

typedef enum
{
    STATE_PLAYING,
    STATE_OVER
} GameState;

/*
 * The snake body is stored in a circular buffer indexed from tail to head.
 *
 *   head   — index of the most recently added cell (current head)
 *   length — number of occupied cells
 *
 * SnakeSegment(g, 0) gives the head, SnakeSegment(g, length-1) the tail.
 * Advancing the snake means incrementing `head` and writing the new cell;
 * the tail implicitly shifts because `length` stays the same (no growth).
 */
typedef struct
{
    Cell body[MAX_SNAKE];
    int head;
    int length;

    Direction dir;
    Direction nextDir; /* buffered input, applied on the next step */

    Cell food;
    int score;

    GameState state;
    CinderTimer moveTimer;
    float moveInterval;
    CinderRng rng;
} Game;

/* =============================================================================
 * Constants
 * ============================================================================= */

static const CinderColor kBgColor = {15, 15, 25, 255};
static const CinderColor kSnakeHead = {120, 230, 100, 255};
static const CinderColor kSnakeBody = {65, 165, 55, 255};
static const CinderColor kFoodColor = {230, 75, 75, 255};
static const CinderColor kFoodGlow = {230, 75, 75, 55};
static const CinderColor kSubtextColor = {160, 160, 160, 255};
static const CinderColor kOverlayColor = {0, 0, 0, 150};

/* =============================================================================
 * Helpers
 * ============================================================================= */

static Cell SnakeSegment(const Game *g, int idx)
{
    return g->body[(g->head - idx + MAX_SNAKE) % MAX_SNAKE];
}

/* Returns true if cell c overlaps any of the first `count` snake segments. */
static bool CellOnSnake(const Game *g, Cell c, int count)
{
    for (int i = 0; i < count; i++)
    {
        Cell s = SnakeSegment(g, i);
        if (s.x == c.x && s.y == c.y)
            return true;
    }
    return false;
}

static void SpawnFood(Game *g)
{
    Cell c;
    do
    {
        c.x = (int)CinderRngUint(&g->rng, GRID_W);
        c.y = (int)CinderRngUint(&g->rng, GRID_H);
    } while (CellOnSnake(g, c, g->length)); /* retry until off the snake */
    g->food = c;
}

static void DrawCenteredText(CinderFont *font, const char *text,
                             int cx, int y, CinderColor color)
{
    CinderSize sz = CinderMeasureText(font, text);
    CinderDrawText(font, text, cx - sz.w / 2, y, color);
}

/* Pixel rect for a grid cell, inset by CELL_PAD for a grid-line effect. */
static CinderRect CellRect(int x, int y)
{
    return (CinderRect){
        x * CELL_SIZE + CELL_PAD,
        y * CELL_SIZE + CELL_PAD,
        CELL_SIZE - CELL_PAD * 2,
        CELL_SIZE - CELL_PAD * 2};
}

/* =============================================================================
 * Game logic
 * ============================================================================= */

static void ResetGame(Game *g)
{
    CinderRngSeedAuto(&g->rng);

    g->length = 4;
    g->head = g->length - 1;
    g->dir = DIR_RIGHT;
    g->nextDir = DIR_RIGHT;
    g->score = 0;
    g->state = STATE_PLAYING;
    g->moveInterval = MOVE_INTERVAL_INIT;

    /* Place snake horizontally in the center, pointing right. */
    int startX = GRID_W / 2 - g->length / 2;
    int startY = GRID_H / 2;
    for (int i = 0; i < g->length; i++)
        g->body[i] = (Cell){startX + i, startY};

    SpawnFood(g);

    g->moveTimer = CinderStartTimer();
    CinderTimerSetRepeat(&g->moveTimer, true);
}

/* Advance the snake one grid step. Sets STATE_OVER on any collision. */
static void StepSnake(Game *g)
{
    g->dir = g->nextDir;

    Cell head = SnakeSegment(g, 0);
    Cell next = head;

    switch (g->dir)
    {
    case DIR_UP:
        next.y--;
        break;
    case DIR_DOWN:
        next.y++;
        break;
    case DIR_LEFT:
        next.x--;
        break;
    case DIR_RIGHT:
        next.x++;
        break;
    }

    /* Wall collision */
    if (next.x < 0 || next.x >= GRID_W || next.y < 0 || next.y >= GRID_H)
    {
        g->state = STATE_OVER;
        return;
    }

    /*
     * Self collision: exclude the current tail (index length-1) because it
     * vacates its cell this same step (unless the snake is about to grow,
     * in which case the tail stays — but that's fine, the new head can't
     * reach the tail in one move).
     */
    if (CellOnSnake(g, next, g->length - 1))
    {
        g->state = STATE_OVER;
        return;
    }

    bool ate = (next.x == g->food.x && next.y == g->food.y);

    /* Advance the head in the circular buffer. */
    g->head = (g->head + 1) % MAX_SNAKE;
    g->body[g->head] = next;

    if (ate)
    {
        g->length++;
        g->score++;

        g->moveInterval -= SPEED_STEP;
        if (g->moveInterval < MOVE_INTERVAL_MIN)
            g->moveInterval = MOVE_INTERVAL_MIN;

        SpawnFood(g);
    }
}

static bool IsOpposite(Direction a, Direction b)
{
    return (a == DIR_UP && b == DIR_DOWN) ||
           (a == DIR_DOWN && b == DIR_UP) ||
           (a == DIR_LEFT && b == DIR_RIGHT) ||
           (a == DIR_RIGHT && b == DIR_LEFT);
}

static void HandleInput(Game *g)
{
    Direction req = g->nextDir;

    /* Accept both arrow keys and WASD. */
    if (CinderIsKeyPressed(CINDER_KEY_UP) || CinderIsKeyPressed(CINDER_KEY_W))
        req = DIR_UP;
    if (CinderIsKeyPressed(CINDER_KEY_DOWN) || CinderIsKeyPressed(CINDER_KEY_S))
        req = DIR_DOWN;
    if (CinderIsKeyPressed(CINDER_KEY_LEFT) || CinderIsKeyPressed(CINDER_KEY_A))
        req = DIR_LEFT;
    if (CinderIsKeyPressed(CINDER_KEY_RIGHT) || CinderIsKeyPressed(CINDER_KEY_D))
        req = DIR_RIGHT;

    /* Ignore a 180-degree reversal — it would immediately cause self-collision. */
    if (!IsOpposite(req, g->dir))
        g->nextDir = req;
}

static void UpdatePlaying(Game *g)
{
    HandleInput(g);

    if (CinderTimerDone(&g->moveTimer, g->moveInterval))
        StepSnake(g);
}

static void UpdateOver(Game *g)
{
    if (CinderIsKeyPressed(CINDER_KEY_ENTER))
        ResetGame(g);
}

/* =============================================================================
 * Rendering
 * ============================================================================= */

static void DrawGame(const Game *g, CinderFont *scoreFont, CinderFont *msgFont)
{
    CinderClearBackground(kBgColor);

    /* Snake body (tail → neck, drawn first so the head renders on top). */
    for (int i = g->length - 1; i >= 1; i--)
    {
        Cell s = SnakeSegment(g, i);
        CinderDrawRoundedRect(CellRect(s.x, s.y), SEG_RADIUS, kSnakeBody);
    }

    /* Head */
    Cell head = SnakeSegment(g, 0);
    CinderDrawRoundedRect(CellRect(head.x, head.y), SEG_RADIUS, kSnakeHead);

    /* Food: glow ring + solid circle */
    int fx = g->food.x * CELL_SIZE + CELL_SIZE / 2;
    int fy = g->food.y * CELL_SIZE + CELL_SIZE / 2;
    CinderDrawCircle(fx, fy, FOOD_GLOW_R, kFoodGlow);
    CinderDrawCircle(fx, fy, FOOD_RADIUS, kFoodColor);

    /* Score */
    char buf[32];
    snprintf(buf, sizeof(buf), "Score: %d", g->score);
    DrawCenteredText(scoreFont, buf, WINDOW_W / 2, 10, CINDER_WHITE);

    /* Controls hint */
    DrawCenteredText(msgFont, "WASD / Arrow keys",
                     WINDOW_W / 2, WINDOW_H - 26, kSubtextColor);

    /* Game-over overlay */
    if (g->state == STATE_OVER)
    {
        CinderDrawRect((CinderRect){0, 0, WINDOW_W, WINDOW_H}, kOverlayColor);

        snprintf(buf, sizeof(buf), "Score: %d", g->score);
        DrawCenteredText(scoreFont, "Game Over",
                         WINDOW_W / 2, WINDOW_H / 2 - 60, CINDER_WHITE);
        DrawCenteredText(scoreFont, buf,
                         WINDOW_W / 2, WINDOW_H / 2 - 10, CINDER_WHITE);
        DrawCenteredText(msgFont, "Press Enter to play again",
                         WINDOW_W / 2, WINDOW_H / 2 + 50, kSubtextColor);
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
    winDesc.title = "Snake";
    winDesc.size.w = WINDOW_W;
    winDesc.size.h = WINDOW_H;
    winDesc.centerX = true;
    winDesc.centerY = true;

    if (CinderCreateWindow(winDesc) != CINDER_STATUS_OK)
        return 1;

    CinderSetBlendMode(CINDER_BLEND_ALPHA);

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

        switch (g.state)
        {
        case STATE_PLAYING:
            UpdatePlaying(&g);
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