#include <grpcpp/grpcpp.h>
#include "messages.grpc.pb.h"
#include "services.grpc.pb.h"

// Placeholder: Minimal gRPC wrapper for C SDK
class GrpcWrapper {
public:
    GrpcWrapper(const std::string& host, int port) {
        // Initialize gRPC channel (insecure for export compliance)
        channel_ = grpc::CreateChannel(host + ":" + std::to_string(port), grpc::InsecureChannelCredentials());
        stub_ = DCFService::NewStub(channel_);
    }
    bool SendMessage(const std::string& data, const std::string& recipient, std::string* response) {
        DCFMessage request;
        request.set_data(data);
        request.set_recipient(recipient);
        grpc::ClientContext context;
        DCFMessage reply;
        grpc::Status status = stub_->SendMessage(&context, request, &reply);
        if (status.ok()) {
            *response = reply.data();
            return true;
        }
        return false;
    }
private:
    std::shared_ptr<grpc::Channel> channel_;
    std::unique_ptr<DCFService::Stub> stub_;
};

// C interface for gRPC
extern "C" {
void* grpc_wrapper_new(const char* host, int port) {
    return new GrpcWrapper(host, port);
}
bool grpc_wrapper_send(void* wrapper, const char* data, const char* recipient, char** response_out) {
    if (!wrapper || !data || !recipient || !response_out) return false;
    GrpcWrapper* gw = static_cast<GrpcWrapper*>(wrapper);
    std::string response;
    bool success = gw->SendMessage(data, recipient, &response);
    if (success) *response_out = strdup(response.c_str());
    return success;
}
void grpc_wrapper_free(void* wrapper) {
    if (wrapper) delete static_cast<GrpcWrapper*>(wrapper);
}
}
