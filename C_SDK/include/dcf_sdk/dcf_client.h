
#ifndef DCF_CLIENT_H
#define DCF_CLIENT_H

#include <stdbool.h>
#include <stdint.h>
#include "dcf_config.h"
#include "dcf_networking.h"
#include "dcf_redundancy.h"
#include "dcf_plugin_manager.h"
#include "dcf_error.h"

typedef struct {
    DCFConfig* config;
    DCFNetworking* net;
    DCFRedundancy* redundancy;
    DCFPluginManager* plugin_mgr;
    bool running;
} DCFClient;

DCFClient* dcf_client_new(void);
DCFError dcf_client_free(DCFClient* client);
DCFError dcf_client_initialize(DCFClient* client, const char* config_path);
DCFError dcf_client_start(DCFClient* client);
DCFError dcf_client_stop(DCFClient* client);
DCFError dcf_client_send_message(DCFClient* client, const char* data, const char* recipient);
DCFError dcf_client_receive_message(DCFClient* client, char** message_out);

#endif

