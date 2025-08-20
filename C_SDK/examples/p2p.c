#include <dcf_sdk/dcf_client.h>
#include <dcf_sdk/dcf_redundancy.h>
#include <stdio.h>
#include <uuid/uuid.h>

int main() {
    DCFClient* client = dcf_client_new();
    if (dcf_client_initialize(client, "config.json") != DCF_SUCCESS) {
        fprintf(stderr, "Init failed: %s\n", dcf_error_str(DCF_ERR_UNKNOWN));
        return 1;
    }
    if (dcf_client_start(client) != DCF_SUCCESS) {
        fprintf(stderr, "Start failed\n");
        return 1;
    }
    uuid_t target;
    uuid_parse("123e4567-e89b-12d3-a456-426614174000", target);
    char* response;
    DCFError err = dcf_client_send_message(client, "P2P Hello!", target, &response);
    if (err == DCF_SUCCESS) {
        printf("Response: %s\n", response);
        free(response);
    } else {
        fprintf(stderr, "Send failed: %s\n", dcf_error_str(err));
    }
    int rtt;
    if (dcf_redundancy_health_check(client->redundancy, target, &rtt) == DCF_SUCCESS) {
        printf("RTT: %d ms\n", rtt);
    }
    dcf_client_stop(client);
    dcf_client_free(client);
    return 0;
}
