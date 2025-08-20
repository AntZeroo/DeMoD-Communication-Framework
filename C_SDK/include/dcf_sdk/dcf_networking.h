#ifndef DCF_NETWORKING_H
#define DCF_NETWORKING_H
#include "dcf_config.h"
#include "dcf_error.h"

typedef struct DCFNetworking DCFNetworking;

DCFNetworking* dcf_networking_new(void);
DCFError dcf_networking_initialize(DCFNetworking* networking, DCFConfig* config);
DCFError dcf_networking_start(DCFNetworking* networking);
DCFError dcf_networking_stop(DCFNetworking* networking);
DCFError dcf_networking_send(DCFNetworking* networking, const uint8_t* data, size_t len, const char* recipient);
DCFError dcf_networking_receive(DCFNetworking* networking, char** message_out, char** sender_out);
void dcf_networking_free(DCFNetworking* networking);
#endif
