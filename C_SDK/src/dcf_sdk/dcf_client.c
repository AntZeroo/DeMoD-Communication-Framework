#include "dcf_client.h"
#include "dcf_plugin_manager.h"
#include "messages.pb-c.h"
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

struct DCFClient {
    cJSON* config;
    PluginManager* plugin_manager;
    DCFRedundancy* redundancy;
    void* grpc_handle; // Opaque gRPC handle
};

DCFClient* dcf_client_new(void) {
    DCFClient* client = calloc(1, sizeof(DCFClient));
    if (!client) return NULL;
    return client;
}

DCFError dcf_client_initialize(DCFClient* client, const char* config_path) {
    if (!client || !config_path) return DCF_ERR_NULL_PTR;

    // Parse config
    FILE* fp = fopen(config_path, "r");
    if (!fp) return DCF_ERR_CONFIG_INVALID;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buffer = calloc(1, size + 1);
    if (!buffer) { fclose(fp); return DCF_ERR_MALLOC_FAIL; }
    fread(buffer, 1, size, fp);
    fclose(fp);
    client->config = cJSON_Parse(buffer);
    free(buffer);
    if (!client->config) return DCF_ERR_CONFIG_INVALID;

    // Initialize plugin manager
    client->plugin_manager = plugin_manager_new();
    if (!client->plugin_manager) return DCF_ERR_MALLOC_FAIL;
    cJSON* plugin = cJSON_GetObjectItem(client->config, "plugins");
    if (plugin) {
        cJSON* transport = cJSON_GetObjectItem(plugin, "transport");
        if (transport && cJSON_IsString(transport)) {
            DCFError err = plugin_manager_load(client->plugin_manager, transport->valuestring);
            if (err != DCF_SUCCESS) return err;
        }
    }

    // Initialize redundancy
    cJSON* peers = cJSON_GetObjectItem(client->config, "peers");
    if (!peers || !cJSON_IsArray(peers)) return DCF_ERR_CONFIG_INVALID;
    int peer_count = cJSON_GetArraySize(peers);
    const char** peer_list = calloc(peer_count, sizeof(char*));
    if (!peer_list) return DCF_ERR_MALLOC_FAIL;
    for (int i = 0; i < peer_count; i++) {
        cJSON* peer = cJSON_GetArrayItem(peers, i);
        peer_list[i] = peer->valuestring;
    }
    int rtt_threshold = cJSON_GetObjectItem(client->config, "group_rtt_threshold")->valueint;
    DCFError err = dcf_redundancy_init(&client->redundancy, peer_list, peer_count, rtt_threshold);
    free(peer_list);
    if (err != DCF_SUCCESS) return err;

    // Initialize gRPC (placeholder)
    client->grpc_handle = NULL; // Actual gRPC init in grpc_wrapper.cpp
    return DCF_SUCCESS;
}

DCFError dcf_client_start(DCFClient* client) {
    if (!client) return DCF_ERR_NULL_PTR;
    // Start gRPC or plugin transport (placeholder)
    return DCF_SUCCESS;
}

DCFError dcf_client_send_message(DCFClient* client, const char* data, const char* recipient, char** response_out) {
    if (!client || !data || !recipient || !response_out) return DCF_ERR_NULL_PTR;

    // Create Protobuf message
    DCFMessage msg = DCF_MESSAGE__INIT;
    uuid_t sender_uuid;
    uuid_generate(sender_uuid);
    char sender_str[37];
    uuid_unparse(sender_uuid, sender_str);
    msg.sender = sender_str;
    msg.recipient = (char*)recipient;
    msg.data.data = (uint8_t*)data;
    msg.data.len = strlen(data);
    msg.timestamp = time(NULL);

    // Serialize
    size_t packed_size = dcf_message__get_packed_size(&msg);
    uint8_t* buffer = calloc(1, packed_size);
    if (!buffer) return DCF_ERR_MALLOC_FAIL;
    dcf_message__pack(&msg, buffer);

    // Send via plugin or gRPC
    ITransport* transport = plugin_manager_get_transport(client->plugin_manager);
    DCFError err = DCF_ERR_NETWORK_FAIL;
    if (transport) {
        if (transport->send(transport, buffer, packed_size, recipient)) {
            uint8_t* response_data;
            size_t response_size;
            response_data = transport->receive(transport, &response_size);
            if (response_data) {
                DCFMessage* response_msg = dcf_message__unpack(NULL, response_size, response_data);
                if (response_msg) {
                    *response_out = strdup(response_msg->data.data);
                    dcf_message__free_unpacked(response_msg, NULL);
                    err = DCF_SUCCESS;
                }
                free(response_data);
            }
        }
    } else {
        // gRPC send (placeholder)
    }
    free(buffer);
    return err;
}

void dcf_client_stop(DCFClient* client) {
    if (!client) return;
    // Stop gRPC or plugin transport (placeholder)
}

void dcf_client_free(DCFClient* client) {
    if (!client) return;
    cJSON_Delete(client->config);
    plugin_manager_free(client->plugin_manager);
    free(client->redundancy); // Handled by dcf_redundancy_free internally
    free(client);
}
