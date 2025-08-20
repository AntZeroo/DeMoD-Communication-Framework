#ifndef DCF_CONFIG_H
#define DCF_CONFIG_H
#include "dcf_error.h"

typedef struct DCFConfig DCFConfig;

DCFConfig* dcf_config_load(const char* path);
DCFError dcf_config_get_mode(DCFConfig* config, DCFMode* mode_out);
DCFError dcf_config_get_node_id(DCFConfig* config, char** id_out);
DCFError dcf_config_get_peers(DCFConfig* config, char*** peers_out, size_t* count_out);
DCFError dcf_config_get_host(DCFConfig* config, char** host_out);
int dcf_config_get_port(DCFConfig* config);
int dcf_config_get_rtt_threshold(DCFConfig* config);
DCFError dcf_config_get_plugin_path(DCFConfig* config, char** path_out);
DCFError dcf_config_update(DCFConfig* config, const char* key, const char* value);
void dcf_config_free(DCFConfig* config);
#endif
