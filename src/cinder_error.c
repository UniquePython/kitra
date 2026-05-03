#include "cinder_internal.h"

static const char *levelToString(CinderLogLevel level)
{
    switch (level)
    {
    case CINDER_LOG_INFO:
        return "INFO";
    case CINDER_LOG_WARNING:
        return "WARNING";
    case CINDER_LOG_ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

static const char *levelColor(CinderLogLevel level)
{
    switch (level)
    {
    case CINDER_LOG_INFO:
        return "\x1b[36m"; // cyan
    case CINDER_LOG_WARNING:
        return "\x1b[1;33m"; // bold yellow
    case CINDER_LOG_ERROR:
        return "\x1b[1;31m"; // bold red
    default:
        return "\x1b[0m";
    }
}

static FILE *levelStream(CinderLogLevel level)
{
    return (level == CINDER_LOG_ERROR) ? stderr : stdout;
}

void CinderSetErrorCallback(CinderErrorCallback cb, void *userdata)
{
    gCinderCtx.callback = cb;
    gCinderCtx.callbackUserdata = userdata;
}

const char *CinderGetError(void)
{
    return gCinderCtx.lastError;
}

void CinderClearError(void)
{
    gCinderCtx.lastError = NULL;
    gCinderCtx.lastLevel = CINDER_LOG_INFO;
}

CinderLogLevel CinderGetLastLevel(void)
{
    return gCinderCtx.lastLevel;
}

bool CinderHasError(void)
{
    return gCinderCtx.lastLevel == CINDER_LOG_ERROR;
}

void CinderLogInternal__(CinderLogLevel level, const char *msg, const char *file, const char *function, int line)
{
    gCinderCtx.lastLevel = level;
    gCinderCtx.lastError = msg;

    if (gCinderCtx.callback)
        gCinderCtx.callback(level, msg, file, function, line, gCinderCtx.callbackUserdata);
}

void CinderDefaultLogCallback(CinderLogLevel level, const char *message, const char *file, const char *function, int line, void *userdata)
{
    (void)userdata;

    fprintf(levelStream(level),
            "%s[%s] %s:%d (%s): %s\x1b[0m\n",
            levelColor(level),
            levelToString(level),
            file,
            line,
            function,
            message);
}