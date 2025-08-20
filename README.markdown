# DeMoD Communications Framework (DCF)

**Version 5.0.0 | August 14, 2025**  
**Developed by DeMoD LLC**  
**Contact:** info@demodllc.example  
**License:** GNU General Public License v3.0 (GPL-3.0)  
 
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)  


## Overview
The DeMoD Communications Framework (DCF) is a free and open-source software (FOSS) framework evolved from the DeMoD Secure Protocol, designed for low-latency, modular, and interoperable data exchange. It supports applications like IoT messaging, real-time gaming synchronization, distributed computing, and edge networking. DCF features a handshakeless design, efficient Protocol Buffers serialization, and a compatibility layer for UDP, TCP, WebSocket, and gRPC transports, enabling seamless peer-to-peer (P2P) networking with self-healing redundancy.

DCF is hardware and language agnostic, supporting embedded devices (e.g., Raspberry Pi), cloud servers, and mobile platforms (Android/iOS) with bindings in Perl, Python, C, C++, JavaScript (Node.js), Go, Rust, Java/Kotlin, and Swift. Version 5.0.0 introduces a plugin system for custom modules and transports, enhancing extensibility. Licensed under GPL-3.0, DCF ensures open-source derivatives. It includes CLI, TUI, server/client logic, and P2P modes, making it versatile for standalone tools, libraries, or networked services.

> **Important**: DCF complies with U.S. export regulations (EAR and ITAR). It avoids encryption to remain export-control-free. Users must ensure custom extensions comply; consult legal experts for specific use cases. DeMoD LLC disclaims liability for non-compliant modifications.

## Features
- **Modularity**: Independent components with standardized APIs; plugin system for custom extensions.
- **Interoperability**: Protocol Buffers and gRPC ensure cross-language (Perl, Python, C, C++, JS, Go, Rust, Java, Swift) and cross-platform compatibility.
- **Low Latency**: Sub-millisecond exchanges with <1% overhead; handshakeless design for real-time applications.
- **Flexibility**: Compatibility layer for UDP, TCP, WebSocket, gRPC, and custom transports via plugins; supports mobile bindings.
- **Usability**: CLI for automation, TUI for monitoring; server, client, and P2P modes with logging.
- **Self-Healing P2P**: Redundant paths and failure detection for robust networking.
- **Open Source**: GPL-3.0 ensures transparency and community contributions.

## Architecture
Below is a high-level overview of the DCF architecture, illustrating its modular components, transport compatibility, and P2P networking capabilities.

```mermaid
graph TD
    A[DCF Framework] --> B[CLI]
    A --> C[TUI]
    A --> D[Networking Layer]
    
    D --> E[Server Mode]
    D --> F[Client Mode]
    D --> G[P2P Mode]
    G --> H[Self-Healing Redundancy]
    H --> I[Peer Discovery]
    H --> J[Failure Detection]
    
    D --> K[Transport Layer]
    K --> L[WebSocket]
    K --> M[UDP]
    K --> N[TCP]
    K --> O[gRPC]
    K --> P[Custom Plugins]
    
    D --> Q[Protocol Buffers]
    Q --> R[Serialization/Deserialization]
    
    A --> S[Language Bindings]
    S --> T[Perl]
    S --> U[Python]
    S --> V[C/C++]
    S --> W[Node.js]
    S --> X[Go]
    S --> Y[Rust]
    S --> Z[Java/Kotlin]
    S --> AA[Swift]
    
    A --> AB[Platform Support]
    AB --> AC[Embedded Devices]
    AB --> AD[Cloud Servers]
    AB --> AE[Mobile: Android/iOS]
```

## Installation
Clone the repository and follow language-specific setup:

```bash
git clone https://github.com/ALH477/DeMoD-Communication-Framework.git
cd DeMoD-Communication-Framework
```

### Prerequisites
- **Perl**: CPAN modules: `JSON`, `IO::Socket::INET`, `Getopt::Long`, `Curses::UI`, `Google::ProtocolBuffers::Dynamic`, `Grpc::XS`, `Module::Pluggable`.
- **Python**: `pip install protobuf grpcio grpcio-tools importlib`.
- **C**: `libprotobuf-c`.
- **C++**: `grpc`, `protobuf`.
- **Node.js**: `npm install @grpc/grpc-js @grpc/proto-loader`.
- **Go**: `go get google.golang.org/grpc github.com/golang/protobuf`.
- **Rust**: `cargo add tonic prost`.
- **Android**: Gradle: `io.grpc:grpc-okhttp`, `io.grpc:grpc-protobuf`, `io.grpc:grpc-stub`.
- **iOS**: Swift Package Manager: `grpc-swift`.

### Build Steps
1. **Generate Protobuf/gRPC Stubs**:
   ```bash
   # Perl/Python
   protoc --perl_out=perl/lib --python_out=python/dcf --grpc_out=python/dcf --plugin=protoc-gen-grpc_python=python -m grpc_tools.protoc messages.proto services.proto
   # C
   protoc --c_out=c/src messages.proto
   # C++
   protoc --cpp_out=cpp/src --grpc_out=cpp/src --plugin=protoc-gen-grpc=grpc_cpp_plugin messages.proto services.proto
   # Node.js
   protoc --js_out=import_style=commonjs:nodejs/src --grpc_out=nodejs/src --plugin=protoc-gen-grpc=grpc_node_plugin messages.proto services.proto
   # Go
   protoc --go_out=go/src --go-grpc_out=go/src messages.proto services.proto
   # Rust (in build.rs)
   tonic-build::compile_protos("proto/services.proto")
   # Android
   protoc --java_out=android/app/src/main --grpc_out=android/app/src/main --plugin=protoc-gen-grpc-java=grpc-java-plugin messages.proto services.proto
   # iOS
   protoc --swift_out=ios/Sources --grpc-swift_out=ios/Sources messages.proto services.proto
   ```
2. **Install Dependencies**:
   - Perl: `cpanm --installdeps .`
   - Python: `pip install -r python/requirements.txt`
   - C: Compile with `gcc` and `libprotobuf-c`.
   - C++: Compile with `g++`, link `grpc`, `protobuf`.
   - Node.js: `npm install`
   - Go: `go build`
   - Rust: `cargo build`
   - Android: Build via Android Studio.
   - iOS: Build via Xcode with SPM.

3. **Plugins**: Place custom plugins in `plugins/` and reference in `config.json`.

## Usage Examples
### Python (gRPC Client)
```python
# python/dcf.py
from dcf.networking import Networking

net = Networking(transport="gRPC", host="localhost", port=50051)
net.send("Hello, DCF!", "recipient_id")
print(net.receive())
```

### C++ (gRPC Server)
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

int main() {
  grpc::ServerBuilder builder;
  Networking service;
  builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();
  return 0;
}
```

### Node.js (gRPC Client)
```javascript
// nodejs/src/client.js
const grpc = require('@grpc/grpc-js');
const protoLoader = require('@grpc/proto-loader');
const packageDefinition = protoLoader.loadSync('services.proto');
const proto = grpc.loadPackageDefinition(packageDefinition).DCFService;

const client = new proto.DCFService('localhost:50051', grpc.credentials.createInsecure());
client.SendMessage({ data: 'Hello, DCF!' }, (err, response) => {
  if (!err) console.log(response.data);
});
```

### Go (gRPC Server)
```go
// go/src/main.go
package main

import (
	"context"
	"net"
	"google.golang.org/grpc"
	pb "path/to/generated/services"
)

type server struct{ pb.UnimplementedDCFServiceServer }

func (s *server) SendMessage(ctx context.Context, in *pb.DCFMessage) (*pb.DCFMessage, error) {
	return &pb.DCFMessage{Data: "Echo: " + in.Data}, nil
}

func main() {
	lis, _ := net.Listen("tcp", ":50051")
	s := grpc.NewServer()
	pb.RegisterDCFServiceServer(s, &server{})
	s.Serve(lis)
}
```

### Rust (gRPC Server)
```rust
// rust/src/main.rs
use tonic::{transport::Server, Request, Response, Status};
use services::dcf_service_server::{DcfService, DcfServiceServer};
use services::{DcfMessage};

#[derive(Default)]
pub struct Networking {}

#[tonic::async_trait]
impl DcfService for Networking {
    async fn send_message(&self, request: Request<DcfMessage>) -> Result<Response<DcfMessage>, Status> {
        let reply = DcfMessage { data: format!("Echo: {}", request.into_inner().data) };
        Ok(Response::new(reply))
    }
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let addr = "[::1]:50051".parse()?;
    let net = Networking::default();
    Server::builder()
        .add_service(DcfServiceServer::new(net))
        .serve(addr)
        .await?;
    Ok(())
}
```

### Android (Kotlin Client)
```kotlin
// android/app/src/main/kotlin/com/example/dcf/DCFClient.kt
import io.grpc.ManagedChannelBuilder
import com.example.dcf.services.DCFServiceGrpc
import com.example.dcf.messages.DCFMessage

class DCFClient(host: String, port: Int) {
    private val channel = ManagedChannelBuilder.forAddress(host, port).usePlaintext().build()
    private val stub = DCFServiceGrpc.newBlockingStub(channel)

    fun sendMessage(data: String, recipient: String): String {
        val request = DCFMessage.newBuilder().setData(data).setRecipient(recipient).build()
        return stub.sendMessage(request).data
    }
}
```

### iOS (Swift Client)
```swift
// ios/DCFClientswift
import GRPC
import NIO
import SwiftProtobuf

class DCFClient {
    private let connection: ClientConnection
    private let client: DCFServiceClient

    init(host: String, port: Int) {
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
        } catch {
            return nil
        }
    }
}
```

### Plugin Example (Python Transport)
```python
# plugins/python/custom_transport.py
class CustomTransport:
    def setup(self, host, port):
        # Custom transport logic
        pass
    def send(self, data, target):
        # Custom send logic
        pass
    def receive(self):
        # Custom receive logic
        pass
```

## Configuration
Create `config.json` based on `config.json.example`:

```json
{
  "transport": "gRPC",
  "host": "localhost",
  "port": 50051,
  "mode": "client",
  "peers": ["localhost:50051"],
  "plugins": {
    "transport": "custom_plugin.CustomTransport"
  }
}
```

## Testing
Run tests with:
- Perl: `prove -r tests/`
- Python: `pytest tests/`
- C/C++: Compile and run `tests/unit/`.
- Node.js: `npm test`
- Go: `go test ./...`
- Rust: `cargo test`
- Mobile: Use Android Studio/Xcode for unit tests.
- Integration: Use Docker for multi-language setups.

## Contributing
Contributions are welcome! Follow these steps:
1. Fork the repo.
2. Create a feature branch (`git checkout -b feature/xyz`).
3. Add tests and code (follow style: `perltidy`, `black`, `ktlint`, `swiftformat`).
4. Submit a PR with a clear description using the [PR template](docs/PR_TEMPLATE.md).
5. Discuss issues via [GitHub Issues](https://github.com/demodllc/dcf/issues) using the [issue template](docs/ISSUE_TEMPLATE.md).

All contributions must adhere to GPL-3.0 and maintain export compliance.

## Documentation
See `docs/dcf_design_spec.md` for detailed architecture, plugin system, and testing guidelines.
