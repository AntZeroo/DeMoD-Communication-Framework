#include "dcf_plugin_manager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char* host;
    int port;
} CustomTransport;

static bool setup(void* self, const char* host, int port) {
    CustomTransport* transport = (CustomTransport*)self;
    transport->host = strdup(host);
    transport->port = port;
    return true;
}

static bool send(void* self, const uint8_t* data, size_t size, const char* target) {
    printf("Custom transport: Sending %.*s to %s\n", (int)size, data, target);
    return true;
}

static uint8_t* receive(void* self, size_t* size) {
    *size = strlen("Custom response");
    uint8_t* response = calloc(1, *size + 1);
    if (!response) return NULL;
    strcpy((char*)response, "Custom response");
    return response;
}

static void destroy(void* self) {
    CustomTransport* transport = (CustomTransport*)self;
    free(transport->host);
    free(transport);
}

ITransport iface = { setup, send, receive, destroy };

void* create_plugin() {
    return calloc(1, sizeof(CustomTransport));
}

const char* get_plugin_version() {
    return "1.0";
}
