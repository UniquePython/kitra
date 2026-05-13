#ifndef CINDER_RNG_H_
#define CINDER_RNG_H_

#include "cinder_types.h"

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief State for a PCG pseudo-random number generator.
 *
 * Holds the internal state and increment for a PCG32 generator. Each
 * @p CinderRng instance is independent, allowing multiple deterministic
 * streams to run concurrently. @p inc must be odd.
 *
 * @see CinderRngSeed, CinderRngNext
 */
typedef struct CinderRng
{
    uint64_t state; /**< Current generator state, advanced on each call. */
    uint64_t inc;   /**< Stream selector — must be odd. */
} CinderRng;

/**
 * @brief Seeds a @p CinderRng with an explicit seed and stream.
 *
 * Initializes @p rng using the PCG32 seeding sequence. @p stream selects
 * an independent output sequence — two generators with different streams
 * and the same @p seed produce entirely different output.
 *
 * @param rng     RNG instance to initialize.
 * @param seed    Initial state seed.
 * @param stream  Stream selector; identifies the output sequence.
 *
 * @see CinderRngSeedAuto, CinderRng
 */
void CinderRngSeed(CinderRng *rng, uint64_t seed, uint64_t stream);

/**
 * @brief Seeds a @p CinderRng automatically from the system clock.
 *
 * Mixes the high-resolution performance counter with wall-clock time to
 * produce a seed and stream that are unique in practice, even when two
 * generators are seeded within the same millisecond.
 *
 * @param rng  RNG instance to initialize.
 *
 * @see CinderRngSeed, CinderRng
 */
void CinderRngSeedAuto(CinderRng *rng);

/**
 * @brief Returns the next raw 32-bit value from a @p CinderRng.
 *
 * Advances the generator state and returns a uniformly distributed
 * unsigned 32-bit integer.
 *
 * @param rng  RNG instance to advance.
 * @return     Next pseudo-random @p uint32_t.
 *
 * @see CinderRngUint, CinderRngFloat
 */
uint32_t CinderRngNext(CinderRng *rng);

/**
 * @brief Returns a uniformly distributed random integer in [0, @p bound).
 *
 * Uses Lemire's nearly-divisionless algorithm to eliminate modulo bias
 * without a division on the fast path.
 *
 * @param rng    RNG instance to advance.
 * @param bound  Exclusive upper bound. Returns @p 0 if @p bound is 0 or 1.
 * @return       Uniform random value in [0, @p bound).
 *
 * @see CinderRngInt, CinderRngNext
 */
uint32_t CinderRngUint(CinderRng *rng, uint32_t bound);

/**
 * @brief Returns a uniformly distributed random integer in [@p min, @p max].
 *
 * @param rng  RNG instance to advance.
 * @param min  Inclusive lower bound.
 * @param max  Inclusive upper bound. Returns @p min if @p min >= @p max.
 * @return     Uniform random value in [@p min, @p max].
 *
 * @see CinderRngUint, CinderRngFloat
 */
int CinderRngInt(CinderRng *rng, int min, int max);

/**
 * @brief Returns a uniformly distributed random float in [0.0, 1.0).
 *
 * @param rng  RNG instance to advance.
 * @return     Uniform random value in [0.0, 1.0).
 *
 * @see CinderRngFloatRange, CinderRngNext
 */
float CinderRngFloat(CinderRng *rng);

/**
 * @brief Returns a uniformly distributed random float in [@p min, @p max).
 *
 * @param rng  RNG instance to advance.
 * @param min  Inclusive lower bound.
 * @param max  Exclusive upper bound.
 * @return     Uniform random value in [@p min, @p max).
 *
 * @see CinderRngFloat, CinderRngInt
 */
float CinderRngFloatRange(CinderRng *rng, float min, float max);

/**
 * @brief Returns a random point within a rectangle.
 *
 * Both axes are sampled uniformly over the interior of @p bounds,
 * inclusive of all edges.
 *
 * @param rng     RNG instance to advance.
 * @param bounds  Rectangle defining the sampling region.
 * @return        Random @p CinderPoint within @p bounds.
 *
 * @see CinderRngInt, CinderRngDirection
 */
CinderPoint CinderRngPoint(CinderRng *rng, CinderRect bounds);

/**
 * @brief Returns a random unit vector.
 *
 * Uses rejection sampling on the unit disk to produce an unbiased
 * direction without trigonometric functions.
 *
 * @param rng  RNG instance to advance.
 * @return     A normalized @p CinderVec2f with length 1.0.
 *
 * @see CinderRngFloat, CinderRngPoint
 */
CinderVec2f CinderRngDirection(CinderRng *rng);

/**
 * @brief Returns @p true with probability @p p.
 *
 * @param rng  RNG instance to advance.
 * @param p    Probability of returning @p true, in [0.0, 1.0].
 * @return     @p true with probability @p p, @p false otherwise.
 *
 * @see CinderRngFloat
 */
bool CinderRngChance(CinderRng *rng, float p);

/**
 * @brief Returns the next raw 32-bit value from the global RNG.
 * @see CinderRngNext
 */
uint32_t CinderRandNext(void);

/**
 * @brief Returns a bounded random integer from the global RNG.
 * @see CinderRngUint
 */
uint32_t CinderRandUint(uint32_t bound);

/**
 * @brief Returns a random integer in [@p min, @p max] from the global RNG.
 * @see CinderRngInt
 */
int CinderRandInt(int min, int max);

/**
 * @brief Returns a random float in [0.0, 1.0) from the global RNG.
 * @see CinderRngFloat
 */
float CinderRandFloat(void);

/**
 * @brief Returns a random float in [@p min, @p max) from the global RNG.
 * @see CinderRngFloatRange
 */
float CinderRandFloatRange(float min, float max);

/**
 * @brief Returns a random point within @p bounds from the global RNG.
 * @see CinderRngPoint
 */
CinderPoint CinderRandPoint(CinderRect bounds);

/**
 * @brief Returns a random unit vector from the global RNG.
 * @see CinderRngDirection
 */
CinderVec2f CinderRandDirection(void);

/**
 * @brief Returns @p true with probability @p p from the global RNG.
 * @see CinderRngChance
 */
bool CinderRandChance(float p);

#endif /* CINDER_RNG_H_ */