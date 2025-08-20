# DeMoD Communications Framework Design Specification (FOSS Shareware Version)

**Version 5.0.0 | August 20, 2025**  
**Developed by DeMoD LLC**  
**Contact:** info@demodllc.example  
**License:** GNU General Public License v3.0 (GPL-3.0)  

## Table of Contents
- [Introduction](#introduction)
- [Compliance with Export Regulations](#compliance-with-export-regulations)
- [Objectives](#objectives)
- [Glossary](#glossary)
- [Protocol Design](#protocol-design)
  - [Modular Components](#modular-components)
  - [Plugin System](#plugin-system)
  - [AUTO Mode and Master Node](#auto-mode-and-master-node)
  - [Abstract Protocol Flow](#abstract-protocol-flow)
  - [Self-Healing P2P Redundancy](#self-healing-p2p-redundancy)
  - [Compatibility Layer](#compatibility-layer)
  - [Protocol Buffers Definition](#protocol-buffers-definition)
  - [gRPC Service Definition](#grpc-service-definition)
  - [JSON Configuration Schema](#json-configuration-schema)
- [Security Considerations](#security-considerations)
- [Performance and Optimization](#performance-and-optimization)
- [Implementation Guidelines](#implementation-guidelines)
  - [Dependencies and Environment Setup](#dependencies-and-environment-setup)
  - [Perl Implementation Example](#perl-implementation-example)
  - [Python Implementation Example](#python-implementation-example)
  - [C Implementation Example](#c-implementation-example)
  - [C++ Implementation Example](#c-implementation-example)
  - [JavaScript (Node.js) Implementation Example](#javascript-nodejs-implementation-example)
  - [Go Implementation Example](#go-implementation-example)
  - [Rust Implementation Example](#rust-implementation-example)
  - [Mobile Bindings](#mobile-bindings)
    - [Android Bindings (Java/Kotlin)](#android-bindings-javakotlin)
    - [iOS Bindings (Swift)](#ios-bindings-swift)
- [SDK Development Guidelines](#sdk-development-guidelines)
- [Interoperability and Testing Guide](#interoperability-and-testing-guide)
  - [Cross-Language Interoperability](#cross-language-interoperability)
  - [Unit and Integration Testing](#unit-and-integration-testing)
  - [P2P and Redundancy Testing](#p2p-and-redundancy-testing)
  - [Hardware Validation](#hardware-validation)
  - [Mobile Testing](#mobile-testing)
  - [Plugin Testing](#plugin-testing)
  - [AUTO Mode Testing](#auto-mode-testing)
- [Repository Structure](#repository-structure)
- [Building, Deploying, and Collaboration](#building-deploying-and-collaboration)
  - [Building Instructions](#building-instructions)
  - [Deployment Guidelines](#deployment-guidelines)
  - [Contribution Guidelines](#contribution-guidelines)

## Introduction
The DeMoD Communications Framework (DCF) is an evolution of the DeMoD Secure Protocol, reimagined as a free and open-source software (FOSS) shareware version. It provides a modular, interoperable, and lightweight foundation for low-latency data exchange in general-purpose applications, including IoT messaging, real-time gaming synchronization, distributed computing, and edge networking. DCF minimizes computational overhead and latency through a handshakeless design, efficient serialization, and a unified compatibility layer that bridges UDP, TCP, WebSocket, and gRPC transports. This enables seamless peer-to-peer (P2P) networking with built-in redundancy mechanisms for self-healing in dynamic environments.

Designed for extensibility, DCF emphasizes hardware and language agnosticism, supporting deployments from resource-constrained embedded devices (e.g., microcontrollers) to high-performance cloud servers and mobile platforms (Android and iOS) via dedicated bindings. Version 5.0.0 introduces enhanced security plugins, expanded language examples for C, C++, JavaScript (Node.js), Go, and Rust, an AUTO mode with master node control for dynamic role assignment, and SDK submodules for streamlined integration. Licensed under GPL-3.0, it ensures that all future iterations remain open source, promoting community-driven development. The framework includes command-line interface (CLI), text user interface (TUI), server/client logic, P2P, and AUTO modes, making it versatile for standalone tools, integrated libraries, or networked services, with mobile bindings and plugins enabling custom on-device usage.

This specification serves as comprehensive documentation for developers, outlining architecture, implementation, testing, and collaboration to facilitate a well-maintained GitHub repository.

## Compliance with Export Regulations
DCF is designed to fully comply with U.S. export regulations, including the Export Administration Regulations (EAR) administered by the Bureau of Industry and Security (BIS) and the International Traffic in Arms Regulations (ITAR) administered by the Directorate of Defense Trade Controls (DDTC). As an open-source framework under GPL-3.0, DCF is publicly available and freely distributable, which excludes it from most ITAR controls per 22 CFR § 120.11 (public domain exclusion). For EAR, DCF does not incorporate encryption or other controlled technologies classified under ECCN 5D002 or similar categories, focusing instead on general-purpose communications without cryptographic primitives. Mobile bindings, plugins, AUTO mode, and new language examples use standard open-source libraries without export-restricted features. Users must ensure their custom extensions do not introduce controlled items; consult legal experts for specific use cases. DeMoD LLC disclaims liability for non-compliant modifications.

## Objectives
- **Modularity**: Components are independent with standardized interfaces (e.g., APIs for send/receive), enabling easy extension, replacement, or maintenance without affecting the core; plugin system allows dynamic loading of custom modules.
- **Interoperability**: Protocol Buffers ensure consistent, binary-efficient serialization across languages (e.g., Perl, Python, C++, Java, Swift, Go, Rust) and platforms; gRPC extends this to advanced RPC, including mobile bindings; plugins support custom formats.
- **Lightweight and Low Latency**: Target sub-millisecond exchanges with <1% overhead; handshakeless design avoids negotiation delays, suitable for real-time applications; gRPC leverages HTTP/2 for efficient multiplexing on mobile; plugins maintain low overhead.
- **Hardware/Language Agnosticism**: Abstract APIs and definitions allow implementation on any hardware (e.g., ARM, x86, mobile SoCs) or language, with no assumptions on OS or runtime; plugin system uses language-specific discovery mechanisms.
- **Dynamic Network Management**: AUTO mode enables nodes to dynamically switch roles (client, server, P2P) under master node control, with JSON-based metrics for AI-driven optimization.
- **Flexibility**: Acts as a compatibility layer for UDP (datagram-oriented), TCP (stream-oriented), WebSocket (web-friendly), and gRPC (RPC-oriented), with P2P self-healing via redundant paths and failure detection; mobile bindings focus on gRPC for efficiency; plugins enable custom transports.
- **Usability**: Built-in CLI for scripting/automation and TUI for interactive monitoring; supports server, client, P2P, and AUTO modes with configurable logging; master node commands for role assignment and configuration; gRPC adds service discovery options; mobile bindings enable integration into apps; plugins configurable via JSON.
- **Open-Source Assurance**: GPL-3.0 licensing mandates source code availability in derivatives, fostering transparency and community contributions.

## Glossary
- **Handshakeless Design**: Immediate data exchange without preliminary negotiation phases, reducing latency.
- **AUTO Mode**: A node operating mode that dynamically switches between client, server, or P2P roles based on master node directives, enabling adaptive network configurations.
- **Master Node**: A specialized node in MASTER_MODE that assigns roles, updates configurations, collects metrics, and optimizes the network topology for AUTO mode nodes.
- **Protocol Buffers (Protobuf)**: Google's language-neutral, platform-neutral, extensible mechanism for serializing structured data.

## Protocol Design
### Modular Components
DCF is structured into independent modules:
- **Networking**: Handles transport (UDP, TCP, WebSocket, gRPC) and communication logic.
- **Serialization**: Uses Protocol Buffers for efficient data encoding/decoding.
- **Redundancy**: Implements P2P self-healing with RTT-based grouping and failover.
- **Plugin Manager**: Loads custom transports or modules dynamically.
- **Configuration**: JSON-based, validated via schema; supports mode, peers, and plugins.
- **Master Node**: Manages AUTO mode nodes, assigning roles and updating configurations.

### Plugin System
Plugins are dynamically loaded modules (e.g., shared libraries in C) implementing interfaces like `ITransport` for custom transports. Version-checked for compatibility.

### AUTO Mode and Master Node
AUTO mode nodes operate in a flexible state, accepting role assignments (client, server, P2P) and configuration updates from a master node. The master node, operating in MASTER_MODE, uses gRPC to:
- Assign roles via `dcf_master_assign_role` (e.g., `{"command": "set_role", "role": "client"}`).
- Update configurations via `dcf_master_update_config` (e.g., `{"command": "update_config", "key": "rtt_threshold", "value": "50"}`).
- Collect metrics (RTT, peer groups) via `dcf_master_collect_metrics` for AI-driven optimization.
- Optimize network topology via `dcf_master_optimize_network`, triggering RTT-based regrouping.

AUTO mode nodes listen for master commands via `dcf_client_receive_message` and apply changes using `dcf_client_set_mode` and `dcf_config_update`.

### Abstract Protocol Flow
1. **Initialization**: Load config (mode, peers, plugins).
2. **Networking**: Establish transport (gRPC default); AUTO mode nodes connect to master.
3. **Master Control**: Master assigns roles/configs to AUTO mode nodes.
4. **Data Exchange**: Serialize messages (Protobuf), send/receive via transport.
5. **Redundancy**: Monitor peers, reroute on failure, group by RTT.
6. **Metrics**: Master collects RTT and group data for optimization.

### Self-Healing P2P Redundancy
Uses Dijkstra’s algorithm with RTT weights for routing. Groups peers by <50ms RTT threshold. Health checks via gRPC; failover within 10s.

### Compatibility Layer
Abstracts UDP, TCP, WebSocket, gRPC, and custom transports via plugins. gRPC is default for reliability and mobile support.

### Protocol Buffers Definition
```proto
syntax = "proto3";
message DCFMessage {
    string sender = 1;
    string recipient = 2;
    bytes data = 3;
    int64 timestamp = 4;
    bool sync = 5;
    uint32 sequence = 6;
    string redundancy_path = 7;
    string group_id = 8;
}
message HealthRequest { string peer = 1; }
message HealthResponse { bool healthy = 1; string status = 2; }
```

### gRPC Service Definition
```proto
service DCFService {
    rpc SendMessage(DCFMessage) returns (DCFMessage);
    rpc ReceiveStream(Empty) returns (stream DCFMessage);
}
```

### JSON Configuration Schema
```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "type": "object",
  "properties": {
    "transport": { "type": "string", "enum": ["gRPC", "WebSocket", "UDP", "TCP"] },
    "host": { "type": "string" },
    "port": { "type": "integer", "minimum": 1, "maximum": 65535 },
    "mode": { "type": "string", "enum": ["client", "server", "p2p", "auto", "master"] },
    "node_id": { "type": "string" },
    "peers": { "type": "array", "items": { "type": "string" } },
    "group_rtt_threshold": { "type": "integer", "minimum": 0 },
    "plugins": { "type": "object", "properties": { "transport": { "type": "string" } } }
  },
  "required": ["transport", "host", "port", "mode"]
}
```

## Security Considerations
DCF avoids encryption to comply with export regulations. Plugins must not introduce controlled technologies. Master node commands in AUTO mode are unauthenticated; users should add authentication (e.g., JWT) in custom extensions. Validate configs with `config.schema.json`.

## Performance and Optimization
- **Latency**: <1ms for local exchanges; gRPC HTTP/2 multiplexing for mobile.
- **CPU**: <5% on Raspberry Pi during health checks.
- **RTT Grouping**: Clusters peers by <50ms threshold; Dijkstra routing optimizes paths.
- **Master Node**: Lightweight role assignment and metrics collection (<1% overhead).

## Implementation Guidelines
### Dependencies and Environment Setup
- C SDK: `libprotobuf-c`, `libuuid`, `libdl`, `libcjson`, `cmake`, `ncurses`.
- Perl: CPAN modules for Protobuf, gRPC, JSON.
- Python: `protobuf`, `grpcio`, `importlib`.
- Others: Language-specific gRPC/Protobuf libraries.

### Perl Implementation Example
```perl
# perl/lib/DCF/Networking.pm
package DCF::Networking;
use Google::ProtocolBuffers::Dynamic;
use Grpc::XS;
sub new {
    my ($class, %args) = @_;
    my $client = Grpc::XS::Client->new($args{host} . ':' . $args{port});
    return bless { client => $client, mode => $args{mode} }, $class;
}
sub send {
    my ($self, $data, $recipient) = @_;
    my $msg = DCF::Message->new({ data => $data, recipient => $recipient });
    my $response = $self->{client}->call('DCFService/SendMessage', $msg);
    return $response->data;
}
1;
```

### Python Implementation Example
```python
# python/dcf/networking.py
import grpc
import messages_pb2
import services_pb2_grpc
class Networking:
    def __init__(self, transport, host, port, mode='p2p'):
        self.channel = grpc.insecure_channel(f'{host}:{port}')
        self.stub = services_pb2_grpc.DCFServiceStub(self.channel)
        self.mode = mode
    def send(self, data, recipient):
        request = messages_pb2.DCFMessage(data=data, recipient=recipient)
        response = self.stub.SendMessage(request)
        return response.data
```

### C Implementation Example
```c
// c_sdk/src/dcf_client.c
#include "dcf_client.h"
DCFClient* dcf_client_new(void) {
    DCFClient* client = calloc(1, sizeof(DCFClient));
    client->current_mode = AUTO_MODE;  // Default to AUTO
    return client;
}
DCFError dcf_client_send_message(DCFClient* client, const char* data, const char* recipient, char** response_out) {
    uint8_t* serialized;
    size_t len;
    DCFError err = dcf_serialize_message(data, client->config->node_id, recipient, &serialized, &len);
    if (err != DCF_SUCCESS) return err;
    err = dcf_networking_send(client->networking, serialized, len, recipient);
    free(serialized);
    if (err == DCF_SUCCESS) err = dcf_networking_receive(client->networking, response_out, NULL);
    return err;
}
```

### C++ Implementation Example
```cpp
// cpp/src/dcf.cpp
#include <grpcpp/grpcpp.h>
#include "services.grpc.pb.h"
class Networking : public DCFService::Service {
    grpc::Status SendMessage(grpc::ServerContext* context, const DCFMessage* request, DCFMessage* response) override {
        response->set_data("Echo: " + request->data());
        return grpc::Status::OK;
    }
};
```

### JavaScript (Node.js) Implementation Example
```javascript
// nodejs/src/dcf.js
const grpc = require('@grpc/grpc-js');
const protoLoader = require('@grpc/proto-loader');
const packageDefinition = protoLoader.loadSync('services.proto');
const proto = grpc.loadPackageDefinition(packageDefinition).DCFService;
class Networking {
    constructor(host, port, mode = 'p2p') {
        this.client = new proto.DCFService(`${host}:${port}`, grpc.credentials.createInsecure());
        this.mode = mode;
    }
    send(data, recipient, callback) {
        this.client.SendMessage({ data, recipient }, (err, response) => callback(err, response ? response.data : null));
    }
}
```

### Go Implementation Example
```go
// go/src/dcf.go
package dcf
import (
    "context"
    "google.golang.org/grpc"
    pb "path/to/generated/services"
)
type Networking struct {
    client pb.DCFServiceClient
    mode   string
}
func NewNetworking(host string, port int, mode string) *Networking {
    conn, _ := grpc.Dial(host+":"+string(port), grpc.WithInsecure())
    return &Networking{client: pb.NewDCFServiceClient(conn), mode: mode}
}
func (n *Networking) Send(data, recipient string) (string, error) {
    resp, err := n.client.SendMessage(context.Background(), &pb.DCFMessage{Data: data, Recipient: recipient})
    if err != nil {
        return "", err
    }
    return resp.Data, nil
}
```

### Rust Implementation Example
```rust
// rust/src/dcf.rs
use tonic::{transport::Channel, Request};
use services::dcf_service_client::DcfServiceClient;
use services::DcfMessage;
pub struct Networking {
    client: DcfServiceClient<Channel>,
    mode: String,
}
impl Networking {
    pub async fn new(host: &str, port: u16, mode: &str) -> Self {
        let client = DcfServiceClient::connect(format!("http://{}:{}", host, port)).await.unwrap();
        Self { client, mode: mode.to_string() }
    }
    pub async fn send(&mut self, data: String, recipient: String) -> Result<String, tonic::Status> {
        let response = self.client.send_message(Request::new(DcfMessage { data, recipient })).await?;
        Ok(response.into_inner().data)
    }
}
```

### Mobile Bindings
#### Android Bindings (Java/Kotlin)
```kotlin
// android/app/src/main/kotlin/com/example/dcf/DCFClient.kt
import io.grpc.ManagedChannelBuilder
import com.example.dcf.services.DCFServiceGrpc
import com.example.dcf.messages.DCFMessage
class DCFClient(host: String, port: Int, plugins: Map<String, String>) {
    private val channel = ManagedChannelBuilder.forAddress(host, port).usePlaintext().build()
    private val stub = DCFServiceGrpc.newBlockingStub(channel)
    private val customTransport: Any? = loadPlugin(plugins["transport"])
    private fun loadPlugin(path: String?): Any? {
        if (path == null) return null
        val clazz = Class.forName(path)
        return clazz.getConstructor().newInstance()
    }
    fun sendMessage(data: String, recipient: String): String {
        val request = DCFMessage.newBuilder().setData(data).setRecipient(recipient).build()
        return stub.sendMessage(request).data
    }
}
```

#### iOS Bindings (Swift)
```swift
// ios/DCFClient.swift
import GRPC
import NIO
import SwiftProtobuf
protocol DCFPlugin { func setup() }
class DCFClient {
    private let connection: ClientConnection
    private let client: DCFServiceClient
    init(host: String, port: Int, pluginPaths: [String: String]) {
        let group = PlatformSupport.makeEventLoopGroup(loopCount: 1)
        connection = ClientConnection.insecure(group: group).connect(host: host, port: port)
        client = DCFServiceClient(channel: connection)
    }
    func sendMessage(data: String, recipient: String) -> String? {
        var request = DCFMessage()
        request.data = data
        request.recipient = recipient
        do {
            let response = try client.sendMessage(request).response.wait()
            return response.data
        } catch { return nil }
    }
}
```

## SDK Development Guidelines
DCF supports SDK development in multiple languages as submodules within the parent repository. SDKs provide high-level abstractions over DCF core, including networking, serialization, redundancy, plugins, and AUTO mode. Each SDK must:
- **Abstraction Layer**: Unified API for modes (client, server, P2P, AUTO, master), with support for gRPC/Protobuf.
- **Modularity**: Separate classes/structs (e.g., `DCFClient`, `DCFRedundancy`, `DCFMaster` in C SDK).
- **P2P Enhancements**: Include RTT-based grouping (cluster by <50ms threshold), optimized Dijkstra routing with RTT weights, and UUID node IDs for efficiency.
- **AUTO Mode**: Support dynamic role switching and master node commands (e.g., `set_role`, `update_config`).
- **Plugins**: Dynamic loading with factory patterns; version-checked (e.g., `get_plugin_version` in C SDK).
- **Error Handling**: Use language-specific mechanisms (e.g., `DCFError` enum in C, exceptions in Python); detailed codes for failures (e.g., `DCF_ERR_NULL_PTR`, `DCF_ERR_MASTER_UNREACHABLE`).
- **Memory Safety**: Valgrind-compatible for C/C++ (e.g., `calloc`, paired `free`, NULL checks); equivalent for other languages.
- **Performance**: <1ms latency, <5% CPU on Pi; measure RTT via gRPC health checks.
- **Compliance**: GPL-3.0; no encryption.

**Example: C SDK** (in `c_sdk/`) uses structs for modularity, integrates gRPC via C++ wrapper (`grpc_wrapper.cpp`), supports RTT grouping in `dcf_redundancy.c`, and includes AUTO mode with master node control via `dcf_master.c`. Future SDKs (e.g., Python, Perl) will mirror this structure, using language-native patterns (e.g., `importlib` for Python plugins).

## Interoperability and Testing Guide
### Cross-Language Interoperability
Generate Protobuf stubs; test Perl sender -> Python receiver via loopback; extend to gRPC calls (e.g., Python client to C SDK server, Android client to iOS server).

### Unit and Integration Testing
Use Perl’s `Test::More`, Python’s `pytest`, C’s Unity for SDKs. Cover: config parsing, serialization, network sends, gRPC methods, plugin loading, RTT grouping, AUTO mode role assignment.

### P2P and Redundancy Testing
Simulate failures; verify rerouting within 10s using gRPC health checks. Test RTT grouping (<50ms clusters).

### Hardware Validation
Test on Raspberry Pi, AWS EC2; measure latency with gRPC benchmarks. Verify <5% CPU usage.

### Mobile Testing
Test Android/iOS bindings in emulators/devices. Verify interoperability and performance under network constraints.

### Plugin Testing
Test dynamic loading (e.g., `libcustom_transport.so` in C SDK); verify integration and failure cases.

### AUTO Mode Testing
Test master node role assignment, config updates, and metrics collection. Verify AUTO mode nodes switch roles correctly and apply configs. Use Docker for multi-node setups with master and AUTO nodes.

## Repository Structure
```
demod-communications-framework/
├── README.md
├── LICENSE  // GPL-3.0
├── messages.proto
├── services.proto  // For gRPC
├── config.schema.json  // For validation
├── config.json.example
├── perl/  // Perl SDK
│   ├── lib/DCF/  // Modules
│   └── dcf.pl  // Entry
├── python/  // Python SDK
│   ├── dcf/  // Modules
│   └── dcf.py  // Entry
├── c_sdk/  // C SDK submodule
│   ├── include/dcf_sdk/  // Headers: dcf_client.h, dcf_error.h, dcf_master.h, etc.
│   ├── src/dcf_sdk/  // Sources: dcf_client.c, dcf_redundancy.c, dcf_master.c, etc.
│   ├── proto/  // messages.proto, services.proto
│   ├── examples/  // p2p.c, master.c
│   ├── plugins/  // custom_transport.c
│   ├── tests/  // test_redundancy.c, test_plugin.c
│   ├── CMakeLists.txt  // Build
│   └── C-SDKreadme.markdown  // Guide
├── cpp/  // C++ SDK
│   ├── src/  // Sources
│   └── dcf.cpp  // Entry
├── nodejs/  // Node.js SDK
│   ├── src/  // Sources
│   └── dcf.js  // Entry
├── go/  // Go SDK
│   ├── src/  // Sources
│   └── dcf.go  // Entry
├── rust/  // Rust SDK
│   ├── src/  // Sources
│   └── Cargo.toml  // Build
├── android/  // Android bindings
├── ios/  // iOS bindings
├── plugins/  // Shared example plugins
├── tests/  // Shared tests
├── docs/
│   ├── CONTRIBUTING.md
│   ├── CODE_OF_CONDUCT.md
│   └── dcf_design_spec.md  // This doc
├── .github/workflows/  // CI/CD
└── Dockerfile  // Container testing
```

## Building, Deploying, and Collaboration
### Building Instructions
1. Clone repo: `git clone --recurse-submodules https://github.com/demodllc/dcf.git`
2. Generate Protobuf/gRPC:
   - Perl/Python: `protoc --perl_out=perl/lib --python_out=python/dcf --grpc_out=python/dcf --plugin=protoc-gen-grpc_python=python -m grpc_tools.protoc messages.proto services.proto`
   - C SDK: `protoc --c_out=c_sdk/src messages.proto`
   - C++: `protoc --cpp_out=cpp/src --grpc_out=cpp/src --plugin=protoc-gen-grpc=grpc_cpp_plugin messages.proto services.proto`
   - Node.js: `protoc --js_out=import_style=commonjs:nodejs/src --grpc_out=nodejs/src --plugin=protoc-gen-grpc=grpc_node_plugin messages.proto services.proto`
   - Go: `protoc --go_out=go/src --go-grpc_out=go/src messages.proto services.proto`
   - Rust: Use `tonic-build` in `build.rs`
   - Android: `protoc --java_out=android/app/src/main --grpc_out=android/app/src/main --plugin=protoc-gen-grpc-java=grpc-java-plugin messages.proto services.proto`
   - iOS: `protoc --swift_out=ios/Sources --grpc-swift_out=ios/Sources messages.proto services.proto`
3. Build SDKs:
   - C SDK: `cd c_sdk && mkdir build && cd build && cmake .. && make`
   - Perl: `cpanm --installdeps .`
   - Python: `pip install -r python/requirements.txt`
   - Others: Follow language-specific build tools.
4. Plugins: Place in `plugins/` or SDK-specific `plugins/` (e.g., `c_sdk/plugins/`).

### Deployment Guidelines
Run as daemon (e.g., systemd); containerize with Docker. For gRPC, expose ports securely. Master nodes should be deployed on reliable hosts. Mobile: Package as libraries/apps. SDKs: Deploy as submodules; C SDK uses `libdcf_sdk.a`.

### Contribution Guidelines
Fork repo, create feature branch, add tests, submit PR. Follow code style (Perl: `perltidy`; Python: `black`; C: `clang-format`). New SDKs encouraged (e.g., Python, Perl); ensure RTT grouping, plugins, AUTO mode, and GPL-3.0 compliance. Discuss via GitHub Issues.