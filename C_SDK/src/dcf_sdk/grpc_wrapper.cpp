#include <grpcpp/grpcpp.h>
#include "messages.grpc.pb.h"
#include "services.grpc.pb.h"
#include <string>

class GrpcWrapper {
public:
    GrpcWrapper(const std::string& host, int port) : server_running_(false) {
        channel_ = grpc::CreateChannel(host + ":" + std::to_string(port), grpc::InsecureChannelCredentials());
        stub_ = DCFService::NewStub(channel_);
    }

    bool StartServer() {
        if (server_running_) return false;
        grpc::ServerBuilder builder;
        builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());
        builder.RegisterService(&service_);
        server_ = builder.BuildAndStart();
        if (!server_) return false;
        server_running_ = true;
        return true;
    }

    bool StopServer() {
        if (!server_running_) return false;
        server_->Shutdown();
        server_running_ = false;
        return true;
    }

    bool Send(const uint8_t* data, size_t len, const std::string& recipient, std::string* response) {
        DCFMessage request;
        request.set_data(std::string((char*)data, len));
        request.set_recipient(recipient);
        grpc::ClientContext context;
        DCFMessage reply;
        grpc::Status status = stub_->SendMessage(&context, request, &reply);
        if (!status.ok()) return false;
        *response = reply.data();
        return true;
    }

    bool Receive(uint8_t** data_out, size_t* len_out, std::string* sender_out) {
        grpc::ClientContext context;
        std::unique_ptr<grpc::ClientReader<DCFMessage>> reader = stub_->ReceiveStream(&context);
        DCFMessage reply;
        if (!reader->Read(&reply)) return false;
        *len_out = reply.data().size();
        *data_out = (uint8_t*)calloc(1, *len_out + 1);
        if (!*data_out) return false;
        memcpy(*data_out, reply.data().c_str(), *len_out);
        *sender_out = reply.sender();
        return true;
    }

private:
    class DCFServiceImpl : public DCFService::Service {
        grpc::Status SendMessage(grpc::ServerContext* context, const DCFMessage* request, DCFMessage* response) override {
            response->set_data("Echo: " + request->data());
            response->set_sender("server");
            return grpc::Status::OK;
        }
        grpc::Status ReceiveStream(grpc::ServerContext* context, const Empty* request, grpc::ServerWriter<DCFMessage>* writer) override {
            DCFMessage msg;
            msg.set_data("Streamed response");
            msg.set_sender("server");
            writer->Write(msg);
            return grpc::Status::OK;
        }
    };

    std::shared_ptr<grpc::Channel> channel_;
    std::unique_ptr<DCFService::Stub> stub_;
    std::unique_ptr<grpc::Server> server_;
    DCFServiceImpl service_;
    bool server_running_;
};

extern "C" {
void* grpc_wrapper_new(const char* host, int port) {
    return new GrpcWrapper(host, port);
}
bool grpc_wrapper_start_server(void* wrapper) {
    if (!wrapper) return false;
    return static_cast<GrpcWrapper*>(wrapper)->StartServer();
}
bool grpc_wrapper_stop_server(void* wrapper) {
    if (!wrapper) return false;
    return static_cast<GrpcWrapper*>(wrapper)->StopServer();
}
bool grpc_wrapper_send(void* wrapper, const uint8_t* data, size_t len, const char* recipient, char** response_out) {
    if (!wrapper || !data || !recipient || !response_out) return false;
    std::string response;
    bool success = static_cast<GrpcWrapper*>(wrapper)->Send(data, len, recipient, &response);
    if (success) *response_out = strdup(response.c_str());
    return success;
}
bool grpc_wrapper_receive(void* wrapper, uint8_t** data_out, size_t* len_out, char** sender_out) {
    if (!wrapper || !data_out || !len_out || !sender_out) return false;
    std::string sender;
    bool success = static_cast<GrpcWrapper*>(wrapper)->Receive(data_out, len_out, &sender);
    if (success) *sender_out = strdup(sender.c_str());
    return success;
}
void grpc_wrapper_free(void* wrapper) {
    if (wrapper) delete static_cast<GrpcWrapper*>(wrapper);
}
}
