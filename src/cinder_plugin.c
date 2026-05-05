#include "cinder_internal.h"

CinderStatus CinderRegisterPlugin(CinderPlugin plugin)
{
    if (gCinderCtx.pluginCount >= CINDER_MAX_PLUGINS)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Maximum plugin count reached");
        return CINDER_STATUS_PLUGIN_MAX_REACHED;
    }

    if (!plugin.name)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Plugin must have a name");
        return CINDER_STATUS_PLUGIN_NO_NAME;
    }

    for (int i = 0; i < gCinderCtx.pluginCount; i++)
    {
        if (strcmp(gCinderCtx.plugins[i].name, plugin.name) == 0)
        {
            CINDER_LOG(CINDER_LOG_ERROR, "A plugin with that name is already registered");
            return CINDER_STATUS_PLUGIN_DUPLICATE_NAME;
        }
    }

    gCinderCtx.plugins[gCinderCtx.pluginCount] = plugin;

    if (plugin.init)
        plugin.init(plugin.userdata);

    gCinderCtx.pluginCount++;

    return CINDER_STATUS_OK;
}

void CinderUnregisterPlugin(const char *name)
{
    if (!name)
        return;

    for (int i = 0; i < gCinderCtx.pluginCount; i++)
    {
        if (strcmp(gCinderCtx.plugins[i].name, name) == 0)
        {
            if (gCinderCtx.plugins[i].shutdown)
                gCinderCtx.plugins[i].shutdown(gCinderCtx.plugins[i].userdata);

            /* Shift remaining plugins down to fill the gap */
            for (int j = i; j < gCinderCtx.pluginCount - 1; j++)
                gCinderCtx.plugins[j] = gCinderCtx.plugins[j + 1];

            gCinderCtx.pluginCount--;
            return;
        }
    }

    CINDER_LOG(CINDER_LOG_WARNING, "Plugin not found");
}