#include "kitra_internal.h"
#include "kitra_rng.h"

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

static uint32_t pcg32_step(KitraRng *rng)
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

void KitraRngSeed(KitraRng *rng, uint64_t seed, uint64_t stream)
{
    /* inc must be odd */
    rng->state = 0u;
    rng->inc = (stream << 1u) | 1u;
    pcg32_step(rng);
    rng->state += seed;
    pcg32_step(rng);
}

void KitraRngSeedAuto(KitraRng *rng)
{
    /* Mix SDL's high-resolution counter with wall-clock time so two
     * generators seeded in the same millisecond still differ. */
    uint64_t seed = SDL_GetPerformanceCounter();
    uint64_t stream = (uint64_t)time(NULL);

    KitraRngSeed(rng, seed, stream);
}

/* -----------------------------------------------------------------------
 * Core output
 * -------------------------------------------------------------------- */

uint32_t KitraRngNext(KitraRng *rng)
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

uint32_t KitraRngUint(KitraRng *rng, uint32_t bound)
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

int KitraRngInt(KitraRng *rng, int min, int max)
{
    if (min >= max)
        return min;

    uint32_t range = (uint32_t)(max - min) + 1u;

    return min + (int)KitraRngUint(rng, range);
}

/* -----------------------------------------------------------------------
 * Floating-point
 * -------------------------------------------------------------------- */

float KitraRngFloat(KitraRng *rng)
{
    /* Multiply by 2^-32 to get [0, 1).  This gives 32 bits of mantissa
     * precision, which is more than a float can represent — fine for games. */
    return (float)(pcg32_step(rng) * (1.0 / 4294967296.0));
}

float KitraRngFloatRange(KitraRng *rng, float min, float max)
{
    return min + KitraRngFloat(rng) * (max - min);
}

/* -----------------------------------------------------------------------
 * Geometry helpers
 * -------------------------------------------------------------------- */

KitraPoint KitraRngPoint(KitraRng *rng, KitraRect bounds)
{
    return (KitraPoint){
        .x = KitraRngInt(rng, bounds.x, bounds.x + bounds.w - 1),
        .y = KitraRngInt(rng, bounds.y, bounds.y + bounds.h - 1),
    };
}

KitraVec2f KitraRngDirection(KitraRng *rng)
{
    /* Rejection sampling on the unit disk — faster than trig and unbiased. */
    for (;;)
    {
        float x = KitraRngFloatRange(rng, -1.0f, 1.0f);
        float y = KitraRngFloatRange(rng, -1.0f, 1.0f);
        float r2 = x * x + y * y;

        if (r2 > 0.0f && r2 <= 1.0f)
        {
            float inv = 1.0f / sqrtf(r2);
            return (KitraVec2f){{x * inv, y * inv}};
        }
    }
}

/* -----------------------------------------------------------------------
 * Misc
 * -------------------------------------------------------------------- */

bool KitraRngChance(KitraRng *rng, float p)
{
    return KitraRngFloat(rng) < p;
}

/* -----------------------------------------------------------------------
 * Global RNG
 * -------------------------------------------------------------------- */

void KitraRandSeed(uint64_t seed, uint64_t stream) { KitraRngSeed(&gKitraCtx.rng, seed, stream); }
void KitraRandSeedAuto(void) { KitraRngSeedAuto(&gKitraCtx.rng); }
uint32_t KitraRandNext(void) { return KitraRngNext(&gKitraCtx.rng); }
uint32_t KitraRandUint(uint32_t bound) { return KitraRngUint(&gKitraCtx.rng, bound); }
int KitraRandInt(int min, int max) { return KitraRngInt(&gKitraCtx.rng, min, max); }
float KitraRandFloat(void) { return KitraRngFloat(&gKitraCtx.rng); }
float KitraRandFloatRange(float min, float max) { return KitraRngFloatRange(&gKitraCtx.rng, min, max); }
KitraPoint KitraRandPoint(KitraRect bounds) { return KitraRngPoint(&gKitraCtx.rng, bounds); }
KitraVec2f KitraRandDirection(void) { return KitraRngDirection(&gKitraCtx.rng); }
bool KitraRandChance(float p) { return KitraRngChance(&gKitraCtx.rng, p); }