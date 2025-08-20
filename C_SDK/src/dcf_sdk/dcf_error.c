#include "dcf_error.h"

const char* dcf_error_str(DCFError err) {
    switch (err) {
        case DCF_SUCCESS: return "Success";
        case DCF_ERR_NULL_PTR: return "Null pointer error";
        case DCF_ERR_MALLOC_FAIL: return "Memory allocation failed";
        case DCF_ERR_CONFIG_INVALID: return "Invalid configuration";
        case DCF_ERR_NETWORK_FAIL: return "Network failure";
        case DCF_ERR_PROTOBUF_FAIL: return "Protobuf serialization error";
        case DCF_ERR_PLUGIN_FAIL: return "Plugin loading error";
        default: return "Unknown error";
    }
}
