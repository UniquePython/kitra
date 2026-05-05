#ifndef CINDER_PLUGIN_H_
#define CINDER_PLUGIN_H_

#include "cinder_status.h"

#include <stdbool.h>

#ifndef CINDER_MAX_PLUGINS
#define CINDER_MAX_PLUGINS 16
#endif

typedef struct CinderPlugin
{
    const char *name;
    void (*init)(void *userdata);
    void (*update)(float dt, void *userdata);
    void (*draw)(void *userdata);
    void (*shutdown)(void *userdata);
    void *userdata;
} CinderPlugin;

CinderStatus CinderRegisterPlugin(CinderPlugin plugin);
void CinderUnregisterPlugin(const char *name);

#endif /* CINDER_PLUGIN_H_ */