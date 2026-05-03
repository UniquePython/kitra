#ifndef CINDER_TIMER_H_
#define CINDER_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct CinderTimer
{
    uint64_t start;
    uint64_t last;
    bool repeat;
} CinderTimer;

CinderTimer CinderStartTimer(void);
void CinderResetTimer(CinderTimer *t);

float CinderGetTimerElapsed(const CinderTimer *t);
bool CinderTimerDone(CinderTimer *t, float duration);
void CinderTimerSetRepeat(CinderTimer *t, bool repeat);

float CinderGetDeltaTime(void);
float CinderGetFPS(void);
void CinderSetTargetFPS(int fps);

#endif /* CINDER_TIMER_H_ */