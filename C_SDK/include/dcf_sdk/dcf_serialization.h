#ifndef DCF_SERIALIZATION_H
#define DCF_SERIALIZATION_H
#include "dcf_error.h"

DCFError dcf_serialize_message(const char* data, const char* sender, const char* recipient, uint8_t** serialized_out, size_t* len_out);
DCFError dcf_serialize_health_request(const char* peer, uint8_t** serialized_out, size_t* len_out);
DCFError dcf_deserialize_message(const uint8_t* data, size_t len, char** message_out, char** sender_out);
#endif
