#include "kitra_internal.h"

static const char *levelToString(KitraLogLevel level)
{
    switch (level)
    {
    case KITRA_LOG_INFO:
        return "INFO";
    case KITRA_LOG_WARNING:
        return "WARNING";
    case KITRA_LOG_ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

static const char *levelColor(KitraLogLevel level)
{
    switch (level)
    {
    case KITRA_LOG_INFO:
        return "\x1b[36m"; // cyan
    case KITRA_LOG_WARNING:
        return "\x1b[1;33m"; // bold yellow
    case KITRA_LOG_ERROR:
        return "\x1b[1;31m"; // bold red
    default:
        return "\x1b[0m";
    }
}

static FILE *levelStream(KitraLogLevel level)
{
    return (level == KITRA_LOG_ERROR) ? stderr : stdout;
}

void KitraSetErrorCallback(KitraErrorCallback cb, void *userdata)
{
    gKitraCtx.log.callback = cb;
    gKitraCtx.log.userdata = userdata;
}

const char *KitraGetError(void)
{
    return gKitraCtx.log.lastError;
}

void KitraClearError(void)
{
    gKitraCtx.log.lastError = NULL;
    gKitraCtx.log.lastLevel = KITRA_LOG_NONE;
}

KitraLogLevel KitraGetLastLevel(void)
{
    return gKitraCtx.log.lastLevel;
}

bool KitraHasError(void)
{
    return gKitraCtx.log.lastLevel == KITRA_LOG_ERROR;
}

void KitraLogInternal__(KitraLogLevel level, const char *msg, const char *file, const char *function, int line)
{
    gKitraCtx.log.lastLevel = level;
    gKitraCtx.log.lastError = msg;

    if (gKitraCtx.log.callback)
        gKitraCtx.log.callback(level, msg, file, function, line, gKitraCtx.log.userdata);
}

void KitraDefaultLogCallback(KitraLogLevel level, const char *message, const char *file, const char *function, int line, void *userdata)
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