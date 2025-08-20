#ifndef DCF_CLIENT_H
#define DCF_CLIENT_H
#include "dcf_config.h"
#include "dcf_networking.h"
#include "dcf_redundancy.h"
#include "dcf_plugin_manager.h"
#include "dcf_error.h"

typedef enum { CLIENT_MODE, SERVER_MODE, P2P_MODE, AUTO_MODE } DCFMode;

typedef struct DCFClient DCFClient;

DCFClient* dcf_client_new(void);
DCFError dcf_client_initialize(DCFClient* client, const char* config_path);
DCFError dcf_client_start(DCFClient* client);
DCFError dcf_client_stop(DCFClient* client);
DCFError dcf_client_send_message(DCFClient* client, const char* data, const char* recipient, char** response_out);
DCFError dcf_client_receive_message(DCFClient* client, char** message_out, char** sender_out);
void dcf_client_free(DCFClient* client);
#endif
