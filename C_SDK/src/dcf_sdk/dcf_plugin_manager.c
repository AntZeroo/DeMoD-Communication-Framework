#include "dcf_plugin_manager.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

struct PluginManager {
    void* handle;
    ITransport* transport;
};

PluginManager* plugin_manager_new(void) {
    PluginManager* manager = calloc(1, sizeof(PluginManager));
    if (!manager) return NULL;
    return manager;
}

DCFError plugin_manager_load(PluginManager* manager, const char* plugin_path) {
    if (!manager || !plugin_path) return DCF_ERR_NULL_PTR;
    manager->handle = dlopen(plugin_path, RTLD_LAZY);
    if (!manager->handle) return DCF_ERR_PLUGIN_FAIL;
    void* (*create_plugin)(void) = dlsym(manager->handle, "create_plugin");
    const char* (*get_version)(void) = dlsym(manager->handle, "get_plugin_version");
    if (!create_plugin || !get_version) {
        dlclose(manager->handle);
        return DCF_ERR_PLUGIN_FAIL;
    }
    if (strcmp(get_version(), "1.0") != 0) {
        dlclose(manager->handle);
        return DCF_ERR_PLUGIN_FAIL;
    }
    manager->transport = create_plugin();
    if (!manager->transport) {
        dlclose(manager->handle);
        return DCF_ERR_PLUGIN_FAIL;
    }
    return DCF_SUCCESS;
}

ITransport* plugin_manager_get_transport(PluginManager* manager) {
    if (!manager) return NULL;
    return manager->transport;
}

void plugin_manager_free(PluginManager* manager) {
    if (!manager) return;
    if (manager->transport && manager->transport->destroy) {
        manager->transport->destroy(manager->transport);
    }
    if (manager->handle) dlclose(manager->handle);
    free(manager);
}
