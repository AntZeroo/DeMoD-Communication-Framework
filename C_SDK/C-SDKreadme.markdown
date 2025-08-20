# C SDK for DeMoD Communications Framework
Version 5.0.0 | August 19, 2025

## Overview
The C SDK provides a modular, low-latency implementation of DCF for client, server, P2P, and AUTO modes using gRPC/Protobuf. Features include RTT-based grouping, Valgrind-compatible memory management, and a plugin system.

## Build
cd C_SDK
mkdir build && cd build
cmake .. && make

## CLI Commands
The `dcf` binary provides a CLI for scripting and operation. All commands support --json for JSON output, facilitating scripting (e.g., parse with jq or Python).

- **dcf init [config_path]**: Initializes with config (default: config.json). Syntax: dcf init path/to/config.json. Example: dcf init config.json --json
- **dcf start**: Starts the DCF instance. Syntax: dcf start. Example: dcf start --json
- **dcf stop**: Stops the DCF instance. Syntax: dcf stop. Example: dcf stop --json
- **dcf status**: Displays status (running, mode, peers). Syntax: dcf status. Example: dcf status --json
- **dcf send [data] [recipient]**: Sends a message. Syntax: dcf send "Hello" "peer1". Example: dcf send "Test" "peer1" --json
- **dcf receive**: Receives a message. Syntax: dcf receive. Example: dcf receive --json
- **dcf health-check [peer]**: Health checks a peer, returning RTT. Syntax: dcf health-check "peer1". Example: dcf health-check "peer1" --json
- **dcf list-peers**: Lists peers with RTT and group ID. Syntax: dcf list-peers. Example: dcf list-peers --json
- **dcf heal [peer]**: Heals network for a peer. Syntax: dcf heal "peer1". Example: dcf heal "peer1" --json
- **dcf version**: Displays version. Syntax: dcf version. Example: dcf version --json
- **dcf benchmark [peer]**: Benchmarks a peer. Syntax: dcf benchmark "peer1". Example: dcf benchmark "peer1" --json
- **dcf group-peers**: Regroups peers by RTT. Syntax: dcf group-peers. Example: dcf group-peers --json
- **dcf simulate-failure [peer]**: Simulates failure. Syntax: dcf simulate-failure "peer1". Example: dcf simulate-failure "peer1" --json
- **dcf log-level [level]**: Sets log level (0=debug, 1=info, 2=error). Syntax: dcf log-level 0. Example: dcf log-level 1 --json
- **dcf load-plugin [path]**: Loads a plugin. Syntax: dcf load-plugin "libcustom.so". Example: dcf load-plugin "libcustom.so" --json
- **dcf tui**: Starts the Text User Interface. Syntax: dcf tui. Example: dcf tui

## Scripting
Use --json for machine-readable output, e.g.:
dcf status --json | jq '.peer_count'
Pipe commands: dcf init config.json && dcf start && dcf send "Hello" "peer1"

## UI
dcf tui launches an interactive ncurses-based interface for real-time monitoring and command execution.
