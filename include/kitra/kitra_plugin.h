#ifndef KITRA_PLUGIN_H_
#define KITRA_PLUGIN_H_

#include "kitra_status.h"

#include <stdbool.h>

/**
 * @brief Maximum number of plugins that can be registered with the engine.
 *
 * Caps the size of the internal plugin registry. Can be overridden by
 * defining @p KITRA_MAX_PLUGINS before including the Kitra header.
 *
 * @see KitraRegisterPlugin
 */
#ifndef KITRA_MAX_PLUGINS
#define KITRA_MAX_PLUGINS 16
#endif

/**
 * @brief Descriptor for a Kitra engine plugin.
 *
 * Bundles a plugin's name, lifecycle callbacks, and userdata into a single
 * struct passed to @p KitraRegisterPlugin. Any callback may be @p NULL if
 * that lifecycle stage is not needed by the plugin.
 *
 * @see KitraRegisterPlugin, KITRA_MAX_PLUGINS
 */
typedef struct KitraPlugin
{
    const char *name;                         /**< Human-readable plugin name, used for logging and identification. */
    void (*init)(void *userdata);             /**< Called once when the plugin is registered. May be @p NULL. */
    void (*update)(float dt, void *userdata); /**< Called every frame with the delta time in seconds. May be @p NULL. */
    void (*draw)(void *userdata);             /**< Called every frame during the draw stage. May be @p NULL. */
    void (*shutdown)(void *userdata);         /**< Called once when the engine shuts down. May be @p NULL. */
    void *userdata;                           /**< Arbitrary pointer passed to every callback. May be @p NULL. */

} KitraPlugin;

/**
 * @brief Registers a plugin with the engine.
 *
 * Validates and adds @p plugin to the internal plugin registry, then
 * immediately invokes its @p init callback if one is provided. Plugins
 * are subsequently ticked each frame in registration order.
 *
 * Registration will fail if the registry is full, the plugin has no name,
 * or a plugin with the same name is already registered.
 *
 * @param plugin  Plugin descriptor to register. The @p name field must be
 *                non-NULL and unique among all registered plugins.
 * @return        @p KITRA_STATUS_OK on success, or one of:
 *                - @p KITRA_STATUS_PLUGIN_MAX_REACHED if the registry is full.
 *                - @p KITRA_STATUS_PLUGIN_NO_NAME if @p plugin.name is @p NULL.
 *                - @p KITRA_STATUS_PLUGIN_DUPLICATE_NAME if a plugin with
 *                  the same name is already registered.
 *
 * @see KitraUnregisterPlugin, KitraPlugin, KITRA_MAX_PLUGINS
 */
KitraStatus KitraRegisterPlugin(KitraPlugin plugin);

/**
 * @brief Unregisters a plugin by name and invokes its shutdown callback.
 *
 * Locates the plugin with the given @p name, calls its @p shutdown callback
 * if one is set, then removes it from the registry. Remaining plugins are
 * compacted to fill the gap, preserving their relative order. Logs a warning
 * if no plugin with the given name is found. Does nothing if @p name is
 * @p NULL.
 *
 * @param name  Name of the plugin to unregister.
 *
 * @see KitraRegisterPlugin, KitraPlugin
 */
void KitraUnregisterPlugin(const char *name);

#endif /* KITRA_PLUGIN_H_ */