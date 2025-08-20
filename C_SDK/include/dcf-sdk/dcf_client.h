#ifndef DCF_CLIENT_H
#define DCF_CLIENT_H
#include "dcf_error.h"
#include "dcf_redundancy.h"

typedef struct DCFClient DCFClient;

DCFClient* dcf_client_new(void);
DCFError dcf_client_initialize(DCFClient* client, const char* config_path);
DCFError dcf_client_start(DCFClient* client);
DCFError dcf_client_send_message(DCFClient* client, const char* data, const char* recipient, char** response_out);
void dcf_client_stop(DCFClient* client);
void dcf_client_free(DCFClient* client);

#endif
