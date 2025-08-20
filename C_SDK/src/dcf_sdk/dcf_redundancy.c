#include "dcf_redundancy.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

struct DCFRedundancy {
    char** peers;
    size_t peer_count;
    int* rtt_cache;
    int rtt_threshold;
    DCFNetworking* networking;
    bool running;
};

DCFRedundancy* dcf_redundancy_new(void) {
    DCFRedundancy* redundancy = calloc(1, sizeof(DCFRedundancy));
    if (!redundancy) return NULL;
    return redundancy;
}

DCFError dcf_redundancy_initialize(DCFRedundancy* redundancy, DCFConfig* config, DCFNetworking* networking) {
    if (!redundancy || !config || !networking) return DCF_ERR_NULL_PTR;
    redundancy->networking = networking;
    DCFError err = dcf_config_get_peers(config, &redundancy->peers, &redundancy->peer_count);
    if (err != DCF_SUCCESS) return err;
    redundancy->rtt_cache = calloc(redundancy->peer_count, sizeof(int));
    if (!redundancy->rtt_cache) return DCF_ERR_MALLOC_FAIL;
    redundancy->rtt_threshold = dcf_config_get_rtt_threshold(config);
    for (size_t i = 0; i < redundancy->peer_count; i++) {
        int rtt;
        err = dcf_redundancy_health_check(redundancy, redundancy->peers[i], &rtt);
        if (err == DCF_SUCCESS) redundancy->rtt_cache[i] = rtt;
        else redundancy->rtt_cache[i] = INT_MAX;
    }
    return DCF_SUCCESS;
}

DCFError dcf_redundancy_start(DCFRedundancy* redundancy) {
    if (!redundancy) return DCF_ERR_NULL_PTR;
    redundancy->running = true;
    return DCF_SUCCESS;
}

DCFError dcf_redundancy_stop(DCFRedundancy* redundancy) {
    if (!redundancy) return DCF_ERR_NULL_PTR;
    redundancy->running = false;
    return DCF_SUCCESS;
}

DCFError dcf_redundancy_get_optimal_route(DCFRedundancy* redundancy, const char* recipient, char** route_out) {
    if (!redundancy || !recipient || !route_out) return DCF_ERR_NULL_PTR;
    if (!redundancy->running) return DCF_ERR_INVALID_STATE;
    int min_rtt = redundancy->rtt_threshold + 1;
    size_t min_idx = 0;
    bool found = false;
    for (size_t i = 0; i < redundancy->peer_count; i++) {
        if (strcmp(redundancy->peers[i], recipient) == 0) continue;
        if (redundancy->rtt_cache[i] < min_rtt) {
            min_rtt = redundancy->rtt_cache[i];
            min_idx = i;
            found = true;
        }
    }
    if (!found) return DCF_ERR_ROUTE_NOT_FOUND;
    *route_out = strdup(redundancy->peers[min_idx]);
    if (!*route_out) return DCF_ERR_MALLOC_FAIL;
    return DCF_SUCCESS;
}

DCFError dcf_redundancy_health_check(DCFRedundancy* redundancy, const char* peer, int* rtt_out) {
    if (!redundancy || !peer || !rtt_out) return DCF_ERR_NULL_PTR;
    if (!redundancy->running) return DCF_ERR_INVALID_STATE;
    uint8_t* health_request = dcf_serialize_health_request(peer);
    if (!health_request) return DCF_ERR_SERIALIZATION_FAIL;
    char* response;
    DCFError err = dcf_networking_send(redundancy->networking, health_request, strlen((char*)health_request), peer);
    free(health_request);
    if (err != DCF_SUCCESS) return err;
    err = dcf_networking_receive(redundancy->networking, &response, NULL);
    if (err != DCF_SUCCESS) return err;
    *rtt_out = rand() % 100; // Mock RTT; replace with real measurement
    free(response);
    return DCF_SUCCESS;
}

void dcf_redundancy_free(DCFRedundancy* redundancy) {
    if (!redundancy) return;
    for (size_t i = 0; i < redundancy->peer_count; i++) free(redundancy->peers[i]);
    free(redundancy->peers);
    free(redundancy->rtt_cache);
    free(redundancy);
}
