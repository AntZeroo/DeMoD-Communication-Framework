# Stage 1: Build the C SDK
FROM alpine:3.20 AS builder

# Install build dependencies (Alpine equivalents for libprotobuf-c-dev, libuuid-dev, libcjson-dev, etc.)
RUN apk add --no-cache \
    git \
    cmake \
    make \
    gcc \
    g++ \
    protobuf-dev \
    protobuf-c-dev \
    grpc-dev \
    grpc-cpp \
    util-linux-dev \
    cjson-dev \
    ncurses-dev \
    linux-headers

# Clone the mono repo (with submodules for C SDK)
RUN git clone --recurse-submodules https://github.com/ALH477/DeMoD-Communication-Framework.git /dcf

# Build the C SDK
WORKDIR /dcf/c_sdk
RUN mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc)

# Stage 2: Runtime image
FROM alpine:3.20

# Install runtime dependencies (minimal set for execution)
RUN apk add --no-cache \
    libstdc++ \
    protobuf-c \
    grpc \
    grpc-cpp \
    util-linux-uuid \
    cjson \
    ncurses-libs

# Copy built artifacts from builder stage
COPY --from=builder /dcf/c_sdk/build/dcf /usr/local/bin/dcf
COPY --from=builder /dcf/c_sdk/build/libdcf_sdk.a /usr/local/lib/
COPY --from=builder /dcf/c_sdk/include/dcf_sdk /usr/local/include/dcf_sdk
COPY --from=builder /dcf/c_sdk/proto /usr/local/include/dcf_proto
COPY --from=builder /dcf/config.json.example /etc/dcf/config.json

# Set up environment (e.g., for plugins and config)
ENV DCF_CONFIG=/etc/dcf/config.json
RUN mkdir -p /etc/dcf/plugins

# Expose default gRPC port (configurable via config.json)
EXPOSE 50051

# Default command: Run TUI for interactive use; override for CLI scripting
CMD ["dcf", "tui"]
