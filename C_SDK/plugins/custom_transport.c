#include <dcf_sdk/dcf_plugin_manager.h>
#include <stdlib.h>

typedef struct { /* Private data */ } CustomTransport;

bool setup(void* self, const char* host, int port) { return true; }
bool send(void* self, const uint8_t* data, size_t size, const char* target) { return true; }
uint8_t* receive(void* self, size_t* size) { *size = 0; return NULL; }
void destroy(void* self) { free(self); }

ITransport iface = {setup, send, receive, destroy};

void* create_plugin() { return calloc(1, sizeof(CustomTransport)); }
const char* get_plugin_version() { return "1.0"; }
