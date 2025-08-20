// cpp/include/dcf/dcf_client.h
#include <grpcpp/grpcpp.h>
#include "services.grpc.pb.h"
#include <nlohmann/json.hpp>
#include <memory>
#include <string>

enum class Mode { CLIENT, SERVER, P2P, AUTO, MASTER };
enum DCFError { DCF_SUCCESS, DCF_ERR_CONFIG_INVALID /* etc. */ };

class DCFException : public std::runtime_error {
public:
    DCFException(DCFError code, const std::string& msg) : std::runtime_error(msg), code_(code) {}
    DCFError code() const { return code_; }
private:
    DCFError code_;
};

class DCFClient {
public:
    DCFClient(const std::string& configPath);
    ~DCFClient();
    void start();
    void stop();
    std::string sendMessage(const std::string& data, const std::string& recipient);
    // AUTO mode: void setMode(Mode newMode);
private:
    std::unique_ptr<DCFService::Stub> stub_;
    std::unique_ptr<grpc::Channel> channel_;
    Mode mode_;
    std::string nodeId_;  // UUID
    // Other members: redundancy_, master_, etc.
};
