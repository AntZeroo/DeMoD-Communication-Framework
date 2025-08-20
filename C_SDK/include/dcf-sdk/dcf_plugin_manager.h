#ifndef DCF_PLUGIN_MANAGER_H
#define DCF_PLUGIN_MANAGER_H
#include "dcf_error.h"

typedef struct {
    bool (*setup)(void* self, const char* host, int port);
    bool (*send)(void* self, const uint8_t* data, size_t size, const char* target);
    uint8_t* (*receive)(void* self, size_t* size);
    void (*destroy)(void* self);
} ITransport;

typedef struct PluginManager PluginManager;

PluginManager* plugin_manager_new(void);
DCFError plugin_manager_load(PluginManager* manager, const char* plugin_path);
ITransport* plugin_manager_get_transport(PluginManager* manager);
void plugin_manager_free(PluginManager* manager);

#endif
