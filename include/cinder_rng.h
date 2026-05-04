#ifndef CINDER_RNG_H_
#define CINDER_RNG_H_

#include "cinder_types.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct CinderRng
{
    uint64_t state;
    uint64_t inc; /* must be odd */
} CinderRng;

/* -----------------------------------------------------------------------
 * Seeding
 * -------------------------------------------------------------------- */

/* Seed with explicit state + stream values. */
void CinderRngSeed(CinderRng *rng, uint64_t seed, uint64_t stream);

/* Seed from the system clock (good enough for games). */
void CinderRngSeedAuto(CinderRng *rng);

/* -----------------------------------------------------------------------
 * Core output — 32-bit unsigned integer, full range
 * -------------------------------------------------------------------- */

uint32_t CinderRngNext(CinderRng *rng);

/* -----------------------------------------------------------------------
 * Bounded integers  (all ranges are inclusive on both ends)
 * -------------------------------------------------------------------- */

/* [0, bound)  — half-open, like a typical array index */
uint32_t CinderRngUint(CinderRng *rng, uint32_t bound);

/* [min, max] */
int CinderRngInt(CinderRng *rng, int min, int max);

/* -----------------------------------------------------------------------
 * Floating-point
 * -------------------------------------------------------------------- */

/* [0.0f, 1.0f) */
float CinderRngFloat(CinderRng *rng);

/* [min, max) */
float CinderRngFloatRange(CinderRng *rng, float min, float max);

/* -----------------------------------------------------------------------
 * Geometry helpers
 * -------------------------------------------------------------------- */

/* Random point inside rect */
CinderPoint CinderRngPoint(CinderRng *rng, CinderRect bounds);

/* Random direction vector (unit length) */
CinderVec2f CinderRngDirection(CinderRng *rng);

/* -----------------------------------------------------------------------
 * Misc
 * -------------------------------------------------------------------- */

/* true with probability p  (0.0 = never, 1.0 = always) */
bool CinderRngChance(CinderRng *rng, float p);

/* -----------------------------------------------------------------------
 * Global RNG
 * -------------------------------------------------------------------- */

uint32_t CinderRandNext(void);
uint32_t CinderRandUint(uint32_t bound);
int CinderRandInt(int min, int max);
float CinderRandFloat(void);
float CinderRandFloatRange(float min, float max);
CinderPoint CinderRandPoint(CinderRect bounds);
CinderVec2f CinderRandDirection(void);
bool CinderRandChance(float p);

#endif /* CINDER_RNG_H_ */