#ifndef CINDER_LOOP_H_
#define CINDER_LOOP_H_

#include <stdbool.h>

bool CinderIsRunning(void);
void CinderRequestQuit(void);

void CinderBeginFrame(void);
void CinderEndFrame(void);

#endif /* CINDER_LOOP_H_ */