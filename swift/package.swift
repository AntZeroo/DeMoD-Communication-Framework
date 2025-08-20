// swift-tools-version:5.9
import PackageDescription
let package = Package(
    name: "DCF-Swift",
    platforms: [.iOS(.v13), .macOS(.v10_15)],
    products: [.library(name: "DCF", targets: ["DCF"])],
    dependencies: [
        .package(url: "https://github.com/grpc/grpc-swift.git", .upToNextMajor(from: "1.0.0")),
        .package(url: "https://github.com/apple/swift-protobuf.git", .upToNextMajor(from: "1.0.0")),
        .package(url: "https://github.com/apple/swift-nio.git", .upToNextMajor(from: "2.0.0"))
    ],
    targets: [
        .target(name: "DCF", dependencies: ["GRPC", "SwiftProtobuf", "NIO"]),
        .testTarget(name: "DCFTests", dependencies: ["DCF"])
    ]
)
