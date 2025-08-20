cmake_minimum_required(VERSION 3.10)
project(DCFCSdk C CXX)
find_package(Protobuf-c REQUIRED)
find_package(UUID REQUIRED)
find_package(CJSON REQUIRED)
find_package(gRPC CONFIG REQUIRED)
add_library(dcf_sdk STATIC
    src/dcf_sdk/dcf_client.c
    src/dcf_sdk/dcf_config.c
    src/dcf_sdk/dcf_networking.c
    src/dcf_sdk/dcf_redundancy.c
    src/dcf_sdk/dcf_serialization.c
    src/dcf_sdk/dcf_plugin_manager.c
    src/dcf_sdk/grpc_wrapper.cpp
    proto/messages.pb-c.c
)
target_include_directories(dcf_sdk PUBLIC include/dcf_sdk proto)
target_link_libraries(dcf_sdk PRIVATE protobuf-c uuid cjson gRPC::grpc++)
add_executable(p2p examples/p2p.c)
target_link_libraries(p2p PRIVATE dcf_sdk)
add_executable(test_redundancy tests/test_redundancy.c)
target_link_libraries(test_redundancy PRIVATE dcf_sdk)
add_executable(test_plugin tests/test_plugin.c)
target_link_libraries(test_plugin PRIVATE dcf_sdk)
