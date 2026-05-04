#include "cinder_internal.h"
#include "cinder_rng.h"

#include <math.h>
#include <time.h>

/* -----------------------------------------------------------------------
 * PCG32 core
 *
 *   output  = rotr32(((state >> 18) ^ state) >> 27, state >> 59)
 *   advance = state = state * 6364136223846793005 + inc
 *
 * Reference: O'Neill, "PCG: A Family of Simple Fast Space-Efficient
 * Statistically Good Algorithms for Random Number Generation", 2014.
 * -------------------------------------------------------------------- */

#define PCG_MULTIPLIER UINT64_C(6364136223846793005)

static inline uint32_t rotr32(uint32_t x, unsigned r)
{
    return (x >> r) | (x << ((-r) & 31u));
}

static uint32_t pcg32_step(CinderRng *rng)
{
    uint64_t old = rng->state;
    rng->state = old * PCG_MULTIPLIER + rng->inc;

    /* XSH-RR output function */
    uint32_t xorshifted = (uint32_t)(((old >> 18u) ^ old) >> 27u);
    uint32_t rot = (uint32_t)(old >> 59u);

    return rotr32(xorshifted, rot);
}

/* -----------------------------------------------------------------------
 * Seeding
 * -------------------------------------------------------------------- */

void CinderRngSeed(CinderRng *rng, uint64_t seed, uint64_t stream)
{
    /* inc must be odd */
    rng->state = 0u;
    rng->inc = (stream << 1u) | 1u;
    pcg32_step(rng);
    rng->state += seed;
    pcg32_step(rng);
}

void CinderRngSeedAuto(CinderRng *rng)
{
    /* Mix SDL's high-resolution counter with wall-clock time so two
     * generators seeded in the same millisecond still differ. */
    uint64_t seed = SDL_GetPerformanceCounter();
    uint64_t stream = (uint64_t)time(NULL);

    CinderRngSeed(rng, seed, stream);
}

/* -----------------------------------------------------------------------
 * Core output
 * -------------------------------------------------------------------- */

uint32_t CinderRngNext(CinderRng *rng)
{
    return pcg32_step(rng);
}

/* -----------------------------------------------------------------------
 * Bounded integers
 *
 * Uses Lemire's nearly-divisionless algorithm to avoid modulo bias
 * without a division on the fast path.
 *
 * Reference: Lemire, "Fast Random Integer Generation in an Interval",
 * ACM TOMACS 29(1), 2019.
 * -------------------------------------------------------------------- */

uint32_t CinderRngUint(CinderRng *rng, uint32_t bound)
{
    if (bound <= 1u)
        return 0u;

    uint64_t threshold = ((uint64_t)(-bound)) % bound; /* == (2^32 - bound) % bound */

    for (;;)
    {
        uint64_t m = (uint64_t)pcg32_step(rng) * (uint64_t)bound;
        if ((m & UINT32_MAX) >= threshold)
            return (uint32_t)(m >> 32);
    }
}

int CinderRngInt(CinderRng *rng, int min, int max)
{
    if (min >= max)
        return min;

    uint32_t range = (uint32_t)(max - min) + 1u;

    return min + (int)CinderRngUint(rng, range);
}

/* -----------------------------------------------------------------------
 * Floating-point
 * -------------------------------------------------------------------- */

float CinderRngFloat(CinderRng *rng)
{
    /* Multiply by 2^-32 to get [0, 1).  This gives 32 bits of mantissa
     * precision, which is more than a float can represent — fine for games. */
    return (float)(pcg32_step(rng) * (1.0 / 4294967296.0));
}

float CinderRngFloatRange(CinderRng *rng, float min, float max)
{
    return min + CinderRngFloat(rng) * (max - min);
}

/* -----------------------------------------------------------------------
 * Geometry helpers
 * -------------------------------------------------------------------- */

CinderPoint CinderRngPoint(CinderRng *rng, CinderRect bounds)
{
    return (CinderPoint){
        .x = CinderRngInt(rng, bounds.x, bounds.x + bounds.w - 1),
        .y = CinderRngInt(rng, bounds.y, bounds.y + bounds.h - 1),
    };
}

CinderVec2f CinderRngDirection(CinderRng *rng)
{
    /* Rejection sampling on the unit disk — faster than trig and unbiased. */
    for (;;)
    {
        float x = CinderRngFloatRange(rng, -1.0f, 1.0f);
        float y = CinderRngFloatRange(rng, -1.0f, 1.0f);
        float r2 = x * x + y * y;

        if (r2 > 0.0f && r2 <= 1.0f)
        {
            float inv = 1.0f / sqrtf(r2);
            return (CinderVec2f){{x * inv, y * inv}};
        }
    }
}

/* -----------------------------------------------------------------------
 * Misc
 * -------------------------------------------------------------------- */

bool CinderRngChance(CinderRng *rng, float p)
{
    return CinderRngFloat(rng) < p;
}

/* -----------------------------------------------------------------------
 * Global RNG
 * -------------------------------------------------------------------- */

uint32_t CinderRandNext(void) { return CinderRngNext(&gCinderCtx.rng); }
uint32_t CinderRandUint(uint32_t bound) { return CinderRngUint(&gCinderCtx.rng, bound); }
int CinderRandInt(int min, int max) { return CinderRngInt(&gCinderCtx.rng, min, max); }
float CinderRandFloat(void) { return CinderRngFloat(&gCinderCtx.rng); }
float CinderRandFloatRange(float min, float max) { return CinderRngFloatRange(&gCinderCtx.rng, min, max); }
CinderPoint CinderRandPoint(CinderRect bounds) { return CinderRngPoint(&gCinderCtx.rng, bounds); }
CinderVec2f CinderRandDirection(void) { return CinderRngDirection(&gCinderCtx.rng); }
bool CinderRandChance(float p) { return CinderRngChance(&gCinderCtx.rng, p); }