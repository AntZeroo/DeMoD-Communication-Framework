#include "dcf_config.h"
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct DCFConfig {
    DCFMode mode;
    char* node_id;
    char** peers;
    size_t peer_count;
    char* host;
    int port;
    int rtt_threshold;
    char* plugin_path;
};

DCFConfig* dcf_config_load(const char* path) {
    if (!path) return NULL;
    FILE* fp = fopen(path, "r");
    if (!fp) return NULL;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buffer = calloc(1, size + 1);
    if (!buffer) { fclose(fp); return NULL; }
    fread(buffer, 1, size, fp);
    fclose(fp);
    cJSON* json = cJSON_Parse(buffer);
    free(buffer);
    if (!json) return NULL;
    DCFConfig* config = calloc(1, sizeof(DCFConfig));
    if (!config) { cJSON_Delete(json); return NULL; }
    cJSON* mode = cJSON_GetObjectItem(json, "mode");
    if (cJSON_IsString(mode)) {
        if (strcmp(mode->valuestring, "client") == 0) config->mode = CLIENT_MODE;
        else if (strcmp(mode->valuestring, "server") == 0) config->mode = SERVER_MODE;
        else if (strcmp(mode->valuestring, "p2p") == 0) config->mode = P2P_MODE;
    }
    cJSON* node_id = cJSON_GetObjectItem(json, "node_id");
    if (cJSON_IsString(node_id)) config->node_id = strdup(node_id->valuestring);
    cJSON* peers = cJSON_GetObjectItem(json, "peers");
    if (cJSON_IsArray(peers)) {
        config->peer_count = cJSON_GetArraySize(peers);
        config->peers = calloc(config->peer_count, sizeof(char*));
        if (!config->peers) { cJSON_Delete(json); dcf_config_free(config); return NULL; }
        for (size_t i = 0; i < config->peer_count; i++) {
            cJSON* peer = cJSON_GetArrayItem(peers, i);
            if (cJSON_IsString(peer)) config->peers[i] = strdup(peer->valuestring);
        }
    }
    cJSON* host = cJSON_GetObjectItem(json, "host");
    if (cJSON_IsString(host)) config->host = strdup(host->valuestring);
    cJSON* port = cJSON_GetObjectItem(json, "port");
    if (cJSON_IsNumber(port)) config->port = port->valueint;
    cJSON* rtt = cJSON_GetObjectItem(json, "group_rtt_threshold");
    if (cJSON_IsNumber(rtt)) config->rtt_threshold = rtt->valueint;
    cJSON* plugins = cJSON_GetObjectItem(json, "plugins");
    if (plugins) {
        cJSON* transport = cJSON_GetObjectItem(plugins, "transport");
        if (cJSON_IsString(transport)) config->plugin_path = strdup(transport->valuestring);
    }
    cJSON_Delete(json);
    return config;
}

DCFError dcf_config_get_mode(DCFConfig* config, DCFMode* mode_out) {
    if (!config || !mode_out) return DCF_ERR_NULL_PTR;
    *mode_out = config->mode;
    return DCF_SUCCESS;
}

DCFError dcf_config_get_node_id(DCFConfig* config, char** id_out) {
    if (!config || !id_out) return DCF_ERR_NULL_PTR;
    *id_out = config->node_id ? strdup(config->node_id) : NULL;
    return DCF_SUCCESS;
}

DCFError dcf_config_get_peers(DCFConfig* config, char*** peers_out, size_t* count_out) {
    if (!config || !peers_out || !count_out) return DCF_ERR_NULL_PTR;
    *count_out = config->peer_count;
    *peers_out = calloc(*count_out, sizeof(char*));
    if (!*peers_out) return DCF_ERR_MALLOC_FAIL;
    for (size_t i = 0; i < *count_out; i++) (*peers_out)[i] = strdup(config->peers[i]);
    return DCF_SUCCESS;
}

DCFError dcf_config_get_host(DCFConfig* config, char** host_out) {
    if (!config || !host_out) return DCF_ERR_NULL_PTR;
    *host_out = config->host ? strdup(config->host) : NULL;
    return DCF_SUCCESS;
}

int dcf_config_get_port(DCFConfig* config) {
    if (!config) return -1;
    return config->port;
}

int dcf_config_get_rtt_threshold(DCFConfig* config) {
    if (!config) return -1;
    return config->rtt_threshold;
}

DCFError dcf_config_get_plugin_path(DCFConfig* config, char** path_out) {
    if (!config || !path_out) return DCF_ERR_NULL_PTR;
    *path_out = config->plugin_path ? strdup(config->plugin_path) : NULL;
    return DCF_SUCCESS;
}

void dcf_config_free(DCFConfig* config) {
    if (!config) return;
    free(config->node_id);
    free(config->host);
    free(config->plugin_path);
    for (size_t i = 0; i < config->peer_count; i++) free(config->peers[i]);
    free(config->peers);
    free(config);
}
