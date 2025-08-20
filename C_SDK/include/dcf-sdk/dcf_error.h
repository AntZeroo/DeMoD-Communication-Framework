#ifndef DCF_ERROR_H
#define DCF_ERROR_H

typedef enum {
    DCF_SUCCESS = 0,
    DCF_ERR_NULL_PTR,
    DCF_ERR_MALLOC_FAIL,
    DCF_ERR_CONFIG_INVALID,
    DCF_ERR_NETWORK_FAIL,
    DCF_ERR_PROTOBUF_FAIL,
    DCF_ERR_PLUGIN_FAIL,
    DCF_ERR_UNKNOWN
} DCFError;

const char* dcf_error_str(DCFError err);

#endif
