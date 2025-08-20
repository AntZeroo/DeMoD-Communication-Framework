#include "dcf_client.h"
#include <stdio.h>
#include <uuid/uuid.h>

int main() {
    DCFClient* client = dcf_client_new();
    if (!client) {
        fprintf(stderr, "Failed to create client: %s\n", dcf_error_str(DCF_ERR_MALLOC_FAIL));
        return 1;
    }
    DCFError err = dcf_client_initialize(client, "config.json");
    if (err != DCF_SUCCESS) {
        fprintf(stderr, "Init failed: %s\n", dcf_error_str(err));
        dcf_client_free(client);
        return 1;
    }
    err = dcf_client_start(client);
    if (err != DCF_SUCCESS) {
        fprintf(stderr, "Start failed: %s\n", dcf_error_str(err));
        dcf_client_free(client);
        return 1;
    }
    char* response;
    err = dcf_client_send_message(client, "P2P Hello!", "123e4567-e89b-12d3-a456-426614174000", &response);
    if (err == DCF_SUCCESS) {
        printf("Response: %s\n", response);
        free(response);
    } else {
        fprintf(stderr, "Send failed: %s\n", dcf_error_str(err));
    }
    char* message, *sender;
    err = dcf_client_receive_message(client, &message, &sender);
    if (err == DCF_SUCCESS) {
        printf("Received from %s: %s\n", sender, message);
        free(message);
        free(sender);
    }
    err = dcf_client_stop(client);
    if (err != DCF_SUCCESS) {
        fprintf(stderr, "Stop failed: %s\n", dcf_error_str(err));
    }
    dcf_client_free(client);
    return 0;
}
