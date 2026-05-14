#include "kitra_internal.h"

KitraStatus KitraRegisterPlugin(KitraPlugin plugin)
{
    if (gKitraCtx.pluginCount >= KITRA_MAX_PLUGINS)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Maximum plugin count reached");
        return KITRA_STATUS_PLUGIN_MAX_REACHED;
    }

    if (!plugin.name)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Plugin must have a name");
        return KITRA_STATUS_PLUGIN_NO_NAME;
    }

    for (int i = 0; i < gKitraCtx.pluginCount; i++)
    {
        if (strcmp(gKitraCtx.plugins[i].name, plugin.name) == 0)
        {
            KITRA_LOG(KITRA_LOG_ERROR, "A plugin with that name is already registered");
            return KITRA_STATUS_PLUGIN_DUPLICATE_NAME;
        }
    }

    gKitraCtx.plugins[gKitraCtx.pluginCount] = plugin;

    if (plugin.init)
        plugin.init(plugin.userdata);

    gKitraCtx.pluginCount++;

    return KITRA_STATUS_OK;
}

void KitraUnregisterPlugin(const char *name)
{
    if (!name)
        return;

    for (int i = 0; i < gKitraCtx.pluginCount; i++)
    {
        if (strcmp(gKitraCtx.plugins[i].name, name) == 0)
        {
            if (gKitraCtx.plugins[i].shutdown)
                gKitraCtx.plugins[i].shutdown(gKitraCtx.plugins[i].userdata);

            /* Shift remaining plugins down to fill the gap */
            for (int j = i; j < gKitraCtx.pluginCount - 1; j++)
                gKitraCtx.plugins[j] = gKitraCtx.plugins[j + 1];

            gKitraCtx.pluginCount--;
            return;
        }
    }

    KITRA_LOG(KITRA_LOG_WARNING, "Plugin not found");
}