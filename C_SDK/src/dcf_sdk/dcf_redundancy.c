#include "dcf_redundancy.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct DCFRedundancy {
    const char** peers;
    size_t peer_count;
    int rtt_threshold;
    int* rtt_values; // Cached RTTs for each peer
};

DCFError dcf_redundancy_init(DCFRedundancy** redundancy, const char** peers, size_t peer_count, int rtt_threshold) {
    if (!redundancy || !peers || peer_count == 0) return DCF_ERR_NULL_PTR;
    DCFRedundancy* r = calloc(1, sizeof(DCFRedundancy));
    if (!r) return DCF_ERR_MALLOC_FAIL;
    r->peers = calloc(peer_count, sizeof(char*));
    if (!r->peers) { free(r); return DCF_ERR_MALLOC_FAIL; }
    r->rtt_values = calloc(peer_count, sizeof(int));
    if (!r->rtt_values) { free(r->peers); free(r); return DCF_ERR_MALLOC_FAIL; }
    for (size_t i = 0; i < peer_count; i++) {
        r->peers[i] = strdup(peers[i]);
        if (!r->peers[i]) {
            for (size_t j = 0; j < i; j++) free((char*)r->peers[j]);
            free(r->peers); free(r->rtt_values); free(r);
            return DCF_ERR_MALLOC_FAIL;
        }
    }
    r->peer_count = peer_count;
    r->rtt_threshold = rtt_threshold;
    *redundancy = r;
    return DCF_SUCCESS;
}

DCFError dcf_redundancy_health_check(DCFRedundancy* redundancy, const char* peer, int* rtt_out) {
    if (!redundancy || !peer || !rtt_out) return DCF_ERR_NULL_PTR;
    // Simulate RTT measurement (gRPC health check placeholder)
    *rtt_out = rand() % 100; // Mock RTT < 100ms
    return DCF_SUCCESS;
}

DCFError dcf_redundancy_measure_rtt(DCFRedundancy* redundancy, const char* peer, int* rtt_out) {
    if (!redundancy || !peer || !rtt_out) return DCF_ERR_NULL_PTR;
    for (size_t i = 0; i < redundancy->peer_count; i++) {
        if (strcmp(redundancy->peers[i], peer) == 0) {
            DCFError err = dcf_redundancy_health_check(redundancy, peer, rtt_out);
            if (err == DCF_SUCCESS) redundancy->rtt_values[i] = *rtt_out;
            return err;
        }
    }
    return DCF_ERR_UNKNOWN;
}

DCFError dcf_redundancy_get_alternate_path(DCFRedundancy* redundancy, const char* target, char** path_out) {
    if (!redundancy || !target || !path_out) return DCF_ERR_NULL_PTR;
    // Simplified Dijkstra’s algorithm: select peer with lowest RTT
    int min_rtt = redundancy->rtt_threshold + 1;
    size_t min_idx = 0;
    for (size_t i = 0; i < redundancy->peer_count; i++) {
        if (strcmp(redundancy->peers[i], target) != 0 && redundancy->rtt_values[i] < min_rtt) {
            min_rtt = redundancy->rtt_values[i];
            min_idx = i;
        }
    }
    if (min_rtt > redundancy->rtt_threshold) return DCF_ERR_NETWORK_FAIL;
    *path_out = strdup(redundancy->peers[min_idx]);
    if (!*path_out) return DCF_ERR_MALLOC_FAIL;
    return DCF_SUCCESS;
}
