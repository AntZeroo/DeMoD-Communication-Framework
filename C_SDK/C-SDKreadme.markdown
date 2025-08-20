# C SDK for DeMoD Communications Framework
**Version 5.0.0 | August 14, 2025**

## Overview
The C SDK provides a lightweight, modular implementation of DCF, supporting client, server, and P2P modes with gRPC/Protobuf. Features RTT-based grouping (<50ms clusters), Valgrind-compatible memory management, and plugins for custom transports.

## Build
```bash
cd c_sdk
mkdir build && cd build
cmake .. && make
```

## Usage
See `examples/p2p.c`. Requires `libprotobuf-c`, `libuuid`, `libcjson`.

## Testing
```bash
make test_redundancy test_plugin
valgrind --leak-check=full ./p2p
```

## Features
- **Error Handling**: Uses `DCFError` enum for detailed errors (e.g., `DCF_ERR_NULL_PTR`).
- **Memory Safety**: Valgrind-compatible with `calloc` and paired `free`.
- **P2P**: RTT-based grouping and Dijkstra routing in `dcf_redundancy.c`.
- **Plugins**: Dynamic loading via `dcf_plugin_manager.c`.

## Structure
- `include/dcf_sdk/`: Headers (`dcf_client.h`, `dcf_error.h`, etc.).
- `src/dcf_sdk/`: Sources (`dcf_client.c`, `grpc_wrapper.cpp`).
- `proto/`: Protobuf-generated files.
- `examples/`: Sample program (`p2p.c`).
- `plugins/`: Custom transport (`custom_transport.c`).
- `tests/`: Unit tests (`test_redundancy.c`, `test_plugin.c`).