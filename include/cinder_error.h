#ifndef CINDER_ERROR_H_
#define CINDER_ERROR_H_

#include <stdbool.h>

typedef enum
{
    CINDER_LOG_NONE,
    CINDER_LOG_INFO,
    CINDER_LOG_WARNING,
    CINDER_LOG_ERROR,

} CinderLogLevel;

typedef void (*CinderErrorCallback)(
    CinderLogLevel level,
    const char *message,
    const char *file,
    const char *function,
    int line,
    void *userdata);

void CinderSetErrorCallback(CinderErrorCallback cb, void *userdata);

const char *CinderGetError(void);
CinderLogLevel CinderGetLastLevel(void);
bool CinderHasError(void);
void CinderClearError(void);

void CinderDefaultLogCallback(CinderLogLevel level, const char *message, const char *file, const char *function, int line, void *userdata);

// Internal — do not call directly
void CinderLogInternal__(CinderLogLevel level, const char *msg, const char *file, const char *function, int line);
#define CINDER_LOG(level, msg) CinderLogInternal__((level), (msg), __FILE__, __func__, __LINE__)

#endif /* CINDER_ERROR_H_ */