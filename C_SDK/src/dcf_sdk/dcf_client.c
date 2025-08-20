#include "dcf_client.h"
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include <stdio.h>

DCFClient* dcf_client_new(void) {
    DCFClient* client = calloc(1, sizeof(DCFClient));
    if (!client) return NULL;
    return client;
}

void dcf_client_free(DCFClient* client) {
    if (!client) return;
    dcf_config_free(client->config);
    dcf_networking_free(client->net);
    dcf_redundancy_free(client->redundancy);
    dcf_plugin_manager_free(client->plugin_mgr);
    free(client);
}

DCFError dcf_client_initialize(DCFClient* client, const char* config_path) {
    if (!client || !config_path) return DCF_ERR_NULL_PTR;
    client->config = dcf_config_load(config_path);
    if (!client->config) return DCF_ERR_CONFIG_INVALID;
    client->net = dcf_networking_new();
    if (!client->net) return DCF_ERR_MALLOC_FAIL;
    DCFError err = dcf_networking_initialize(client->net, client->config);
    if (err != DCF_SUCCESS) return err;
    client->plugin_mgr = dcf_plugin_manager_new();
    if (!client->plugin_mgr) return DCF_ERR_MALLOC_FAIL;
    err = dcf_plugin_manager_initialize(client->plugin_mgr, client->config);
    if (err != DCF_SUCCESS) return err;
    client->redundancy = dcf_redundancy_new();
    if (!client->redundancy) return DCF_ERR_MALLOC_FAIL;
    err = dcf_redundancy_initialize(client->redundancy, client->config);
    if (err != DCF_SUCCESS) return err;
    return DCF_SUCCESS;
}

DCFError dcf_client_start(DCFClient* client) {
    if (!client) return DCF_ERR_NULL_PTR;
    client->running = true;
    return DCF_SUCCESS;
}

void dcf_client_stop(DCFClient* client) {
    if (!client) return;
    client->running = false;
}

DCFError dcf_client_send_message(DCFClient* client, const char* data, const char* recipient, char** response_out) {
    if (!client || !data || !recipient || !response_out) return DCF_ERR_NULL_PTR;
    if (!client->running) return DCF_ERR_INVALID_STATE;
    // Get node ID
    char* node_id = dcf_config_get_node_id(client->config);
    if (!node_id) return DCF_ERR_CONFIG_INVALID;
    // Serialize message
    DCFMessage msg = DCF_MESSAGE__INIT;
    msg.sender = strdup(node_id);
    msg.recipient = strdup(recipient);
    msg.data.len = strlen(data);
    msg.data.data = (uint8_t*)strdup(data);
    msg.timestamp = time(NULL);
    msg.has_sync = 1;
    msg.sync = true;
    msg.has_sequence = 1;
    msg.sequence = rand() % UINT32_MAX;
    // Pack
    size_t len = dcf_message__get_packed_size(&msg);
    uint8_t* packed = calloc(1, len);
    if (!packed) {
        free(msg.sender);
        free(msg.recipient);
        free(msg.data.data);
        free(node_id);
        return DCF_ERR_MALLOC_FAIL;
    }
    dcf_message__pack(&msg, packed);
    free(msg.sender);
    free(msg.recipient);
    free(msg.data.data);
    free(node_id);
    // Send via networking
    DCFError err = dcf_networking_send(client->net, packed, len, recipient);
    free(packed);
    if (err != DCF_SUCCESS) return err;
    // Receive response
    uint8_t* response_data;
    size_t response_len;
    err = dcf_networking_receive(client->net, &response_data, &response_len);
    if (err != DCF_SUCCESS) return err;
    DCFMessage* response_msg = dcf_message__unpack(NULL, response_len, response_data);
    free(response_data);
    if (!response_msg) return DCF_ERR_DESERIALIZATION_FAIL;
    *response_out = strdup((char*)response_msg->data.data);
    dcf_message__free_unpacked(response_msg, NULL);
    return DCF_SUCCESS;
}

DCFError dcf_client_receive_message(DCFClient* client, char** message_out, char** sender_out) {
    if (!client || !message_out || !sender_out) return DCF_ERR_NULL_PTR;
    if (!client->running) return DCF_ERR_INVALID_STATE;
    uint8_t* data;
    size_t len;
    DCFError err = dcf_networking_receive(client->net, &data, &len);
    if (err != DCF_SUCCESS) return err;
    DCFMessage* msg = dcf_message__unpack(NULL, len, data);
    free(data);
    if (!msg) return DCF_ERR_DESERIALIZATION_FAIL;
    *message_out = strdup((char*)msg->data.data);
    *sender_out = strdup(msg->sender);
    dcf_message__free_unpacked(msg, NULL);
    return DCF_SUCCESS;
}
