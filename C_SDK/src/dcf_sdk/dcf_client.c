
#include "dcf_client.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <uuid/uuid.h>

DCFClient* dcf_client_new(void) {
    DCFClient* client = calloc(1, sizeof(DCFClient));
    if (!client) return NULL;
    return client;
}

DCFError dcf_client_free(DCFClient* client) {
    if (!client) return DCF_ERR_NULL_PTR;
    if (client->config) dcf_config_free(client->config);
    if (client->net) dcf_networking_free(client->net);
    if (client->redundancy) dcf_redundancy_free(client->redundancy);
    if (client->plugin_mgr) dcf_plugin_manager_free(client->plugin_mgr);
    free(client);
    return DCF_SUCCESS;
}

DCFError dcf_client_initialize(DCFClient* client, const char* config_path) {
    if (!client || !config_path) return DCF_ERR_NULL_PTR;
    DCFError err;
    client->config = dcf_config_load(config_path);
    if (!client->config) return DCF_ERR_CONFIG_INVALID;
    client->net = dcf_networking_new();
    if (!client->net) return DCF_ERR_MALLOC_FAIL;
    err = dcf_networking_initialize(client->net, client->config);
    if (err != DCF_SUCCESS) return err;
    client->plugin_mgr = dcf_plugin_manager_new();
    if (!client->plugin_mgr) return DCF_ERR_MALLOC_FAIL;
    err = dcf_plugin_manager_initialize(client->plugin_mgr, client->config);
    if (err != DCF_SUCCESS) return err;
    client->redundancy = dcf_redundancy_new(client->config);
    if (!client->redundancy) return DCF_ERR_MALLOC_FAIL;
    err = dcf_redundancy_initialize(client->redundancy, client->config);
    if (err != DCF_SUCCESS) return err;
    return DCF_SUCCESS;
}

DCFError dcf_client_start(DCFClient* client) {
    if (!client) return DCF_ERR_NULL_PTR;
    client->running = true;
    DCFError err = dcf_networking_start(client->net);
    if (err != DCF_SUCCESS) return err;
    err = dcf_redundancy_start(client->redundancy);
    if (err != DCF_SUCCESS) return err;
    return DCF_SUCCESS;
}

DCFError dcf_client_stop(DCFClient* client) {
    if (!client) return DCF_ERR_NULL_PTR;
    client->running = false;
    DCFError err = dcf_networking_stop(client->net);
    if (err != DCF_SUCCESS) return err;
    err = dcf_redundancy_stop(client->redundancy);
    if (err != DCF_SUCCESS) return err;
    return DCF_SUCCESS;
}

DCFError dcf_client_send_message(DCFClient* client, const char* data, const char* recipient) {
    if (!client || !data || !recipient) return DCF_ERR_NULL_PTR;
    if (!client->running) return DCF_ERR_INVALID_STATE;
    char* serialized = dcf_serialize_message(data, recipient, client->config->node_id);
    if (!serialized) return DCF_ERR_SERIALIZATION_FAIL;
    DCFError err;
    if (client->config->mode == P2P_MODE) {
        char* route = dcf_redundancy_get_optimal_route(client->redundancy, recipient);
        if (!route) {
            free(serialized);
            return DCF_ERR_ROUTE_NOT_FOUND;
        }
        err = dcf_networking_send(client->net, serialized, strlen(serialized), route);
        free(route);
    } else {
        err = dcf_networking_send(client->net, serialized, strlen(serialized), recipient);
    }
    free(serialized);
    return err;
}

DCFError dcf_client_receive_message(DCFClient* client, char** message_out) {
    if (!client || !message_out) return DCF_ERR_NULL_PTR;
    if (!client->running) return DCF_ERR_INVALID_STATE;
    char* received = dcf_networking_receive(client->net);
    if (!received) return DCF_ERR_NETWORK_FAIL;
    *message_out = dcf_deserialize_message(received);
    free(received);
    if (!*message_out) return DCF_ERR_DESERIALIZATION_FAIL;
    return DCF_SUCCESS;
}
