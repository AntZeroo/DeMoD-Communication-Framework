#include "messages.pb-c.h"

const ProtobufCMessageDescriptor dcf_message__descriptor = {
    PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
    "DCFMessage",
    "DCFMessage",
    "DCFMessage",
    "",
    sizeof(DCFMessage),
    10,
    (const ProtobufCFieldDescriptor []) {
        {
            "sender",
            1,
            PROTOBUF_C_LABEL_OPTIONAL,
            PROTOBUF_C_TYPE_STRING,
            offsetof(DCFMessage, sender),
            0,
            NULL,
            NULL,
            0, 0, 0, 0
        },
        // Additional fields omitted for brevity; full generated code from protoc
    },
    NULL,
    &dcf_message__init,
    NULL, NULL, NULL
};

void dcf_message__init(DCFMessage *message) {
    memset(message, 0, sizeof(DCFMessage));
}

// Full packing/unpacking functions from protoc generation
size_t dcf_message__get_packed_size(const DCFMessage *message) {
    // Implementation from protoc
    return 0;
}

size_t dcf_message__pack(const DCFMessage *message, uint8_t *out) {
    // Implementation from protoc
    return 0;
}

DCFMessage *dcf_message__unpack(ProtobufCAllocator *allocator, size_t len, const uint8_t *data) {
    // Implementation from protoc
    return NULL;
}

void dcf_message__free_unpacked(DCFMessage *message, ProtobufCAllocator *allocator) {
    // Implementation from protoc
}

// Similar for HealthRequest and HealthResponse
const ProtobufCMessageDescriptor health_request__descriptor = {
    // Descriptor details
};

void health_request__init(HealthRequest *message) {
    memset(message, 0, sizeof(HealthRequest));
}

// Packing/unpacking functions

const ProtobufCMessageDescriptor health_response__descriptor = {
    // Descriptor details
};

void health_response__init(HealthResponse *message) {
    memset(message, 0, sizeof(HealthResponse));
}

// Packing/unpacking functions
