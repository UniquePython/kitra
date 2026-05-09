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

#define FONT_SIZE_HUD 18
#define FONT_SIZE_SMALL 14

#define WINDOW_W 800
#define WINDOW_H 600

/* The pivot is fixed at the upper-centre of the window. */
#define PIVOT_X (WINDOW_W / 2)
#define PIVOT_Y 110

#define GRAVITY 9.81f /* m/s²                           */
#define PPM 200.0f    /* pixels per metre (display scale) */

/* Adjustable parameters and their ranges */
#define LENGTH_INIT 1.0f
#define LENGTH_MIN 0.3f
#define LENGTH_MAX 2.5f
#define LENGTH_STEP 0.1f

#define DAMPING_INIT 0.1f /* coefficient b in θ'' = -(g/L)sin θ - b·θ' */
#define DAMPING_MIN 0.0f
#define DAMPING_MAX 2.0f
#define DAMPING_STEP 0.05f

/* Visual */
#define BOB_RADIUS 14
#define BOB_GLOW_R 26
#define PIVOT_RADIUS 6
#define ROD_WIDTH 2

#define TRAIL_LEN 350 /* positions kept in the trail circular buffer */
#define SUBSTEPS 8    /* RK4 substeps per rendered frame             */

/* HUD */
#define HUD_X 14
#define HUD_Y 14
#define HUD_W 216
#define HUD_ROW_H 24
#define HUD_PANEL_H 272

/* =============================================================================
 * Types
 * ============================================================================= */

/* Full physical state: angle and angular velocity. */
typedef struct
{
    float theta, omega;
} State;

/* Time-derivative of the state (used internally by RK4). */
typedef struct
{
    float dtheta, domega;
} Deriv;

typedef struct
{
    State s;
    float length;  /* metres */
    float damping; /* b coefficient, 1/s */

    bool paused;
    bool dragging;

    /* Bob trail: circular buffer of pixel positions. */
    int trailHead;
    int trailCount;
    float trailX[TRAIL_LEN];
    float trailY[TRAIL_LEN];
} Sim;

/* =============================================================================
 * Colors
 * ============================================================================= */

static const CinderColor kBgColor = {15, 15, 25, 255};
static const CinderColor kPanelColor = {0, 0, 0, 190};
static const CinderColor kSepColor = {70, 70, 100, 200};
static const CinderColor kLabelColor = {135, 135, 155, 255};
static const CinderColor kValueColor = {220, 220, 235, 255};
static const CinderColor kPivotColor = {190, 190, 200, 255};
static const CinderColor kPivotRing = {0, 0, 0, 220};
static const CinderColor kRodColor = {190, 195, 210, 255};
static const CinderColor kBobColor = {80, 155, 255, 255};
static const CinderColor kBobGlow = {80, 155, 255, 45};
static const CinderColor kHintColor = {100, 100, 120, 255};
static const CinderColor kPauseColor = {255, 205, 75, 255};

/* =============================================================================
 * Physics — fourth-order Runge–Kutta
 * =============================================================================
 *
 * Equation of motion:
 *
 *   θ''(t) = -(g/L) · sin θ  −  b · θ'
 *
 * This is the exact (non-linearised) equation, so large angles are handled
 * correctly. The small-angle approximation is only used for the period display.
 */

static Deriv DerivAt(State s, float L, float b)
{
    return (Deriv){
        .dtheta = s.omega,
        .domega = -(GRAVITY / L) * sinf(s.theta) - b * s.omega};
}

static State RK4Step(State s, float L, float b, float dt)
{
    Deriv k1 = DerivAt(s, L, b);

    State s2 = {s.theta + k1.dtheta * dt * 0.5f,
                s.omega + k1.domega * dt * 0.5f};
    Deriv k2 = DerivAt(s2, L, b);

    State s3 = {s.theta + k2.dtheta * dt * 0.5f,
                s.omega + k2.domega * dt * 0.5f};
    Deriv k3 = DerivAt(s3, L, b);

    State s4 = {s.theta + k3.dtheta * dt,
                s.omega + k3.domega * dt};
    Deriv k4 = DerivAt(s4, L, b);

    float c = dt / 6.0f;
    return (State){
        s.theta + c * (k1.dtheta + 2.0f * k2.dtheta + 2.0f * k3.dtheta + k4.dtheta),
        s.omega + c * (k1.domega + 2.0f * k2.domega + 2.0f * k3.domega + k4.domega)};
}

/* =============================================================================
 * Helpers
 * ============================================================================= */

/* Pixel position of the bob's centre. */
static void BobPos(const Sim *sim, float *bx, float *by)
{
    *bx = PIVOT_X + sinf(sim->s.theta) * sim->length * PPM;
    *by = PIVOT_Y + cosf(sim->s.theta) * sim->length * PPM;
}

static void PushTrail(Sim *sim, float bx, float by)
{
    sim->trailHead = (sim->trailHead + 1) % TRAIL_LEN;
    sim->trailX[sim->trailHead] = bx;
    sim->trailY[sim->trailHead] = by;
    if (sim->trailCount < TRAIL_LEN)
        sim->trailCount++;
}

static void ClearTrail(Sim *sim)
{
    sim->trailHead = 0;
    sim->trailCount = 0;
}

static void Reset(Sim *sim)
{
    sim->s = (State){(float)CINDER_DEG2RAD(45.0), 0.0f};
    sim->length = LENGTH_INIT;
    sim->damping = DAMPING_INIT;
    sim->paused = false;
    sim->dragging = false;
    ClearTrail(sim);
}

/* =============================================================================
 * Update
 * ============================================================================= */

static void Update(Sim *sim, float dt)
{
    /* ---- Parameter controls -------------------------------------------- */

    if (CinderIsKeyPressed(CINDER_KEY_UP) || CinderIsKeyPressed(CINDER_KEY_W))
    {
        sim->length += LENGTH_STEP;
        if (sim->length > LENGTH_MAX)
            sim->length = LENGTH_MAX;
        ClearTrail(sim);
    }
    if (CinderIsKeyPressed(CINDER_KEY_DOWN) || CinderIsKeyPressed(CINDER_KEY_S))
    {
        sim->length -= LENGTH_STEP;
        if (sim->length < LENGTH_MIN)
            sim->length = LENGTH_MIN;
        ClearTrail(sim);
    }
    if (CinderIsKeyPressed(CINDER_KEY_RIGHT) || CinderIsKeyPressed(CINDER_KEY_D))
    {
        sim->damping += DAMPING_STEP;
        if (sim->damping > DAMPING_MAX)
            sim->damping = DAMPING_MAX;
    }
    if (CinderIsKeyPressed(CINDER_KEY_LEFT) || CinderIsKeyPressed(CINDER_KEY_A))
    {
        sim->damping -= DAMPING_STEP;
        if (sim->damping < DAMPING_MIN)
            sim->damping = DAMPING_MIN;
    }
    if (CinderIsKeyPressed(CINDER_KEY_SPACE))
        sim->paused = !sim->paused;
    if (CinderIsKeyPressed(CINDER_KEY_R))
        Reset(sim);

    /* ---- Mouse drag ---------------------------------------------------- *
     * Click and hold the bob to reposition the pendulum interactively.
     * Releasing sets ω = 0 and lets the simulation take over.              */

    CinderPoint mouse = CinderGetMousePos();
    float bx, by;
    BobPos(sim, &bx, &by);

    float dx = (float)mouse.x - bx;
    float dy = (float)mouse.y - by;
    float pickRadSq = (float)((BOB_RADIUS + 10) * (BOB_RADIUS + 10));

    if (CinderIsMouseButtonPressed(CINDER_MOUSE_LEFT) && dx * dx + dy * dy < pickRadSq)
        sim->dragging = true;

    if (!CinderIsMouseButtonDown(CINDER_MOUSE_LEFT))
        sim->dragging = false;

    if (sim->dragging)
    {
        /*
         * atan2(x, y) — note the swapped argument order — gives the angle
         * measured clockwise from the downward vertical, which is exactly θ.
         */
        float mx = (float)mouse.x - PIVOT_X;
        float my = (float)mouse.y - PIVOT_Y;
        sim->s.theta = atan2f(mx, my);
        sim->s.omega = 0.0f;
        ClearTrail(sim);
    }

    /* ---- Physics integration ------------------------------------------- *
     * Split each frame into SUBSTEPS to keep the integrator well inside its
     * stability region even when the pendulum moves quickly.                */

    if (!sim->paused && !sim->dragging)
    {
        float subdt = dt / (float)SUBSTEPS;
        for (int i = 0; i < SUBSTEPS; i++)
            sim->s = RK4Step(sim->s, sim->length, sim->damping, subdt);
    }

    /* Record current bob position for the trail. */
    BobPos(sim, &bx, &by);
    PushTrail(sim, bx, by);
}

/* =============================================================================
 * Rendering
 * ============================================================================= */

static void DrawTrail(const Sim *sim)
{
    /*
     * Walk the circular buffer from oldest entry to newest, fading in both
     * alpha and dot size as we approach the current position.
     */
    for (int i = 0; i < sim->trailCount; i++)
    {
        int idx = (sim->trailHead - sim->trailCount + 1 + i + TRAIL_LEN) % TRAIL_LEN;
        float t = (float)i / (float)sim->trailCount; /* 0 = oldest, 1 = newest */

        uint8_t alpha = (uint8_t)(t * t * 170.0f);
        int r = (t > 0.75f) ? 2 : 1;

        CinderDrawCircle(
            (int)sim->trailX[idx], (int)sim->trailY[idx], r,
            (CinderColor){80, 155, 255, alpha});
    }
}

static void DrawHUD(const Sim *sim, CinderFont *hudFont, CinderFont *smallFont)
{
    /* Background panel */
    CinderDrawRoundedRect(
        (CinderRect){HUD_X - 8, HUD_Y - 8, HUD_W + 16, HUD_PANEL_H},
        7, kPanelColor);

    /* --- Data rows ------------------------------------------------------- */

    float thetaDeg = sim->s.theta * (180.0f / (float)CINDER_PI);

    /*
     * Energy is computed per unit mass (J/kg) so we don't need to assume
     * a bob mass. KE = ½ L² ω², PE = gL(1 − cos θ), both relative to the
     * pivot height.
     */
    float ke = 0.5f * sim->length * sim->length * sim->s.omega * sim->s.omega;
    float pe = GRAVITY * sim->length * (1.0f - cosf(sim->s.theta));
    float energy = ke + pe;

    /* Small-angle period: T = 2π√(L/g). Valid when |θ| is small. */
    float period = 2.0f * (float)CINDER_PI * sqrtf(sim->length / GRAVITY);

    struct
    {
        const char *label;
        float val;
        const char *unit;
    } rows[] = {
        {"Angle", thetaDeg, "deg"},
        {"Velocity", sim->s.omega, "rad/s"},
        {"Length", sim->length, "m"},
        {"Damping", sim->damping, "1/s"},
        {"Period\xe2\x80\xa0", period, "s"}, /* † */
        {"Energy", energy, "J/kg"},
    };
    int rowCount = (int)(sizeof(rows) / sizeof(rows[0]));

    char buf[48];
    int y = HUD_Y;

    for (int i = 0; i < rowCount; i++, y += HUD_ROW_H)
    {
        CinderDrawText(hudFont, rows[i].label, HUD_X, y, kLabelColor);
        snprintf(buf, sizeof(buf), "%.3f %s", rows[i].val, rows[i].unit);
        CinderSize sz = CinderMeasureText(hudFont, buf);
        CinderDrawText(hudFont, buf, HUD_X + HUD_W - sz.w, y, kValueColor);
    }

    /* Separator */
    y += 6;
    CinderDrawLine(HUD_X, y, HUD_X + HUD_W, y, kSepColor);
    y += 8;

    /* --- Key hints ------------------------------------------------------- */

    const char *hints[] = {
        "Drag bob to reposition",
        "Space \xe2\x80\x94 pause / resume",
        "Up / Down \xe2\x80\x94 length",
        "Left / Right \xe2\x80\x94 damping",
        "R \xe2\x80\x94 reset",
    };
    for (int i = 0; i < 5; i++, y += 18)
        CinderDrawText(smallFont, hints[i], HUD_X, y, kHintColor);

    y += 4;
    CinderDrawText(smallFont, "\xe2\x80\xa0 small-angle approximation", HUD_X, y, kHintColor);

    /* Pause banner */
    if (sim->paused)
    {
        CinderSize sz = CinderMeasureText(hudFont, "PAUSED");
        CinderDrawText(hudFont, "PAUSED",
                       WINDOW_W / 2 - sz.w / 2, WINDOW_H - 38,
                       kPauseColor);
    }
}

static void DrawSim(const Sim *sim, CinderFont *hudFont, CinderFont *smallFont)
{
    CinderClearBackground(kBgColor);

    DrawTrail(sim);

    float bx, by;
    BobPos(sim, &bx, &by);

    /* Rod */
    CinderDrawLineThick(PIVOT_X, PIVOT_Y, (int)bx, (int)by, ROD_WIDTH, kRodColor);

    /* Pivot: dark halo so the rod doesn't bleed into it */
    CinderDrawCircle(PIVOT_X, PIVOT_Y, PIVOT_RADIUS + 3, kPivotRing);
    CinderDrawCircle(PIVOT_X, PIVOT_Y, PIVOT_RADIUS, kPivotColor);

    /* Bob: glow ring + solid circle */
    CinderDrawCircle((int)bx, (int)by, BOB_GLOW_R, kBobGlow);
    CinderDrawCircle((int)bx, (int)by, BOB_RADIUS, kBobColor);

    DrawHUD(sim, hudFont, smallFont);
}

/* =============================================================================
 * Entry point
 * ============================================================================= */

int main(void)
{
    if (CinderInit(CINDER_SUBSYSTEM_ALL) != CINDER_STATUS_OK)
        return 1;

    CinderWindowDesc winDesc = CinderDefaultWindowDesc();
    winDesc.title = "Single Pendulum";
    winDesc.size.w = WINDOW_W;
    winDesc.size.h = WINDOW_H;
    winDesc.centerX = true;
    winDesc.centerY = true;

    if (CinderCreateWindow(winDesc) != CINDER_STATUS_OK)
        return 1;

    CinderSetBlendMode(CINDER_BLEND_ALPHA);

    CinderFont *hudFont = CinderLoadFont(FONT_PATH, FONT_SIZE_HUD);
    CinderFont *smallFont = CinderLoadFont(FONT_PATH, FONT_SIZE_SMALL);

    if (!hudFont || !smallFont)
    {
        CinderDestroyFont(&hudFont);
        CinderDestroyFont(&smallFont);
        CinderQuit();
        return 1;
    }

    CinderSetTargetFPS(60);

    Sim sim = {0};
    Reset(&sim);

    while (CinderIsRunning())
    {
        CinderBeginFrame();

        if (CinderIsKeyPressed(CINDER_KEY_ESCAPE))
            CinderRequestQuit();

        Update(&sim, CinderGetDeltaTime());
        DrawSim(&sim, hudFont, smallFont);

        CinderEndFrame();
    }

    CinderDestroyFont(&hudFont);
    CinderDestroyFont(&smallFont);
    CinderQuit();
    return 0;
}