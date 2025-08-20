#include <dcf_sdk/dcf_redundancy.h>
#include <stdio.h>
#include <string.h>

int main() {
    DCFRedundancy* redundancy;
    const char* peers[] = {"localhost:50051", "localhost:50052"};
    DCFError err = dcf_redundancy_init(&redundancy, peers, 2, 50);
    if (err != DCF_SUCCESS) {
        printf("Init failed: %s\n", dcf_error_str(err));
        return 1;
    }
    int rtt;
    err = dcf_redundancy_health_check(redundancy, peers[0], &rtt);
    if (err != DCF_SUCCESS || rtt < 0) {
        printf("Health check failed: %s\n", dcf_error_str(err));
        return 1;
    }
    char* path;
    err = dcf_redundancy_get_alternate_path(redundancy, peers[0], &path);
    if (err == DCF_SUCCESS) {
        printf("Alternate path: %s\n", path);
        free(path);
    }
    // Cleanup handled by dcf_client_free
    printf("Redundancy tests passed\n");
    return 0;
}
