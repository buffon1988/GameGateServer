# GateServer

A production-grade TCP gateway/proxy service for game servers, built on the **muduo** C++ networking library.

## Overview

GateServer is a lightweight, high-performance gateway service that sits between game clients and backend logic servers. It handles client connections, message routing, session management, and security validation — acting as the single entry point for all client traffic.

### Key Features

- **Bidirectional Message Routing**: Smart routing based on player state (lobby → LogicManager, table → LogicServer)
- **Session Management**: Per-user authentication with UUID + random key + message ordering anti-replay
- **Security**: MD5 signature verification on every client message
- **Dynamic Backend Discovery**: Auto-discovers LogicServer instances via LogicManager broadcasts
- **Heartbeat Monitoring**: 30-second heartbeat to all backend connections with auto-reconnect
- **Idle Detection**: Automatic cleanup of unauthenticated connections and inactive users
- **Broadcast Support**: Forward promotional messages (horse/rider banners) to all connected clients

## Architecture

```
                    ┌─────────────┐
                    │   Clients   │ (Mobile/PC Game Clients)
                    └──────┬──────┘
                           │ TCP
                    ┌──────▼──────┐
                    │  GateServer │
                    │  (this)     │
                    └──────┬──────┘
                           │
              ┌────────────┼────────────┐
              │            │            │
     ┌────────▼──┐  ┌──────▼──────┐ ┌───▼────────┐
     │LogicServer│  │LogicServer  │ │ ...        │  (Game Logic Servers)
     └───────────┘  └─────────────┘ └────────────┘
              ▲
              │
     ┌────────▼──────────┐
     │  LogicManager     │  (Backend Orchestrator)
     └───────────────────┘
```

### Threading Model

| Thread | Count | Purpose |
|--------|-------|---------|
| Main IO | 1 | Accepts client connections, runs muduo EventLoop |
| Sub IO | 4 | Handles client I/O read/write |
| Backend | 1 | Connects to LogicManager and LogicServers (auto-reconnect) |
| Business | 1 | Message processing queue (BlockingQueue → handler dispatch) |

## Installation

### Prerequisites

- **OS**: Linux (Ubuntu 16.04+, CentOS 7+) or macOS
- **Compiler**: GCC 4.8+ or Clang with C++11 support
- **CMake**: 3.5+
- **System Libraries**: pthread, rt, dl (usually included in libc)

### Build

```bash
# Clone the repository
git clone <your-repo-url>
cd GateServer

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make -j$(nproc)

# The binary will be at: bin/GateServer
```

> **Note**: The third-party libraries (muduo, OpenSSL) are pre-compiled for **Linux**. If you're on macOS, you may need to recompile muduo for your platform or cross-compile.

### Third-Party Dependencies

All dependencies are vendored in the `3rdParty/` directory:

| Library | Purpose |
|---------|---------|
| **muduo** | C++ async network library (Netty-style) |
| **msgpack** | Binary serialization for message protocol |
| **OpenSSL** | MD5 hashing for message integrity |

No system-level installation required — all libraries are pre-compiled static archives.

## Configuration

Copy the example config and edit:

```bash
cp config/gateserver_config.ini.example config/gateserver_config.ini
```

### Config Options

```ini
# TCP listen port for client connections
ListenPort = 8115

# GateServer identification
GateServerId = 1

# GateServer's own IP (advertised to backend)
GateServerIp = 127.0.0.1

# Authentication key for backend connections
GateServerKey = change_me_to_a_secure_key

# LogicManager connection
LogicManagerIp = 127.0.0.1
LogicManagerPort = 11015
```

## Quick Start

### 1. Build

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 2. Configure

Copy the example config and edit at least `LogicManagerIp` / `LogicManagerPort` to match your environment:

```bash
cp config/gateserver_config.ini.example config/gateserver_config.ini
# edit config/gateserver_config.ini
```

### 3. Run

```bash
# From the project root (so config/ is accessible)
./build/bin/GateServer

# Or from the bin directory
cd build/bin
./GateServer
```

### 4. Verify

Check the log output for:
- `GateServer registered with LogicManager` — backend connection established
- `LogicServer list updated` — backend servers discovered
- Client connections appearing on the configured `ListenPort`

### Config File Search Order

GateServer searches for the config file in this order:
1. `./config/gateserver_config.ini` (relative to working directory)
2. `config/gateserver_config.ini`
3. `../config/gateserver_config.ini`
4. Falls back to hardcoded defaults if none found

## Usage

```bash
# Run from the project root (so config/ is accessible)
./bin/GateServer

# Or specify working directory
cd /path/to/where/config/is
../bin/GateServer
```

## Protocol

### Wire Format

```
|-- 2 bytes bodyLen --|-- 2 bytes checksum --|-- 4 bytes timestamp --|-- 4 bytes msgId --|
|------------------------------------------------------------------------------|--- N bytes body ---|
```

### Client Message Format

```
|-- 2 bytes verifyHeadLen --|-- msgpack verifyHead --|-- 2 bytes entityLen --|-- 1 byte packed --|-- msgpack entity --|
```

The verifyHead contains:
- `m_msgId`: Message type ID
- `m_strUserId`: Player UUID
- `m_strVerifyKey`: Per-session authentication key
- `m_lMsgOrder`: Monotonically increasing sequence number
- `m_iMd5Begin/End`: Byte range for MD5 verification
- `m_strMd5`: MD5 signature of the body

### Security

1. **MD5 Integrity**: Every client message body is signed with MD5. The server verifies the signature using a shared `MD5_SIGN` constant.
2. **Session Key**: On login, the server generates a 32-character random key per client. This key must match in every subsequent message.
3. **Anti-Replay**: Each message carries a sequence number (`m_lMsgOrder`). Messages with order ≤ last seen are rejected.
4. **Timeout**: Unauthenticated connections are dropped after 30 seconds. Inactive authenticated users are kicked after 30 seconds.

## Message Types

### Client → Server

| Msg ID | Message | Description |
|--------|---------|-------------|
| 170 | `MSG_C_2_S_LOGIN_GATE` | Client login to Gate |
| 171 | `MSG_S_2_C_LOGIN_GATE` | Gate response with session key |
| 15 | `MSG_C_2_S_HEART` | Client heartbeat |
| 16 | `MSG_S_2_C_HEART` | Server heartbeat response |
| 180 | `MSG_S_2_C_SHUTDOWN_GATE` | Server disconnects client |

### Server ↔ Backend

| Msg ID | Message | Direction | Description |
|--------|---------|-----------|-------------|
| 12001 | `MSG_G_2_LMG_LOGIN` | Gate → LM | Gate registration |
| 12010 | `MSG_LMG_2_G_SYNC_LOGIC` | LM → Gate | LogicServer list update |
| 12011 | `MSG_LMG_2_G_HORSE_INFO` | LM → Gate | Promotional broadcast |
| 8000 | `MSG_G_2_L_USER_MSG` | Gate → Logic | Forwarded client message |
| 8001 | `MSG_L_2_G_USER_MSG` | Logic → Gate | Forwarded to client |
| 8006 | `MSG_L_2_G_MODIFY_USER_STATE` | Logic → Gate | User state change |

## Development

### Adding a New Game Handler

This GateServer is designed to work with any game backend that follows the same protocol. To integrate:

1. Configure the `LogicManagerIp` and `LogicManagerPort` to point to your backend orchestrator
2. Ensure your backend sends `MSG_LMG_2_G_SYNC_LOGIC` with `LogicInfo` structs containing `m_id`, `m_ip`, `m_port`
3. Ensure your backend sends `MSG_LMG_2_G_HORSE_INFO` for broadcast messages
4. The GateServer will automatically discover and connect to all listed LogicServers

### Extending the Gateway

The core message router is in `GateServer/Business.cpp`. To add new functionality:

1. Define new message IDs in `include/CommonDefine.h`
2. Add message structs in `include/LMsgSS.h`
3. Register the message in `include/LMsg.cpp::InitMsg()`
4. Add a case in `Business::handleMsg()` to dispatch the new message type

## Project Structure

```
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── include/                # Shared base library (minimal set for GateServer)
│   ├── LMsg.h/cpp          # Message framework and factory
│   ├── LMsgSS.h            # Server-to-server message definitions
│   ├── LMsgCS.h            # Client-to-server message definitions
│   ├── ClientNet.h/cpp     # Outbound TCP client wrapper
│   ├── ServerNet.h/cpp     # Inbound TCP server wrapper
│   ├── Decode.h            # Protocol decoder (msgpack/MD5)
│   ├── BaseFunc.h/cpp      # Utilities (MD5, rand, parsing)
│   ├── IniConfig.h/cpp     # INI configuration parser
│   ├── CommonDefine.h      # Global constants and enums
│   ├── GameDefine.h        # Game-specific constants
│   └── ...                 # Other base components
├── GateServer/             # GateServer module
│   ├── main.cpp            # Entry point
│   ├── Work.h/cpp          # Lifecycle orchestration
│   ├── Business.h/cpp      # Core message routing logic
│   ├── Config.h/cpp        # Configuration loading
│   └── GateUser.h/cpp      # Per-client session management
├── 3rdParty/               # Vendored third-party libraries
│   ├── muduo/              # C++ async network library
│   ├── msgpack/            # Binary serialization
│   └── openssl/            # Cryptography (MD5)
└── config/                 # Configuration files
    └── gateserver_config.ini.example
```

## License

This project is derived from the GameServers project and is released under the MIT License. See the original project for details.

## Notes

- This is a **gateway-only** extraction from the larger GameServers project
- The full GameServers project includes: LoginServer, CenterServer, LogicManager, LogicServer, and LogicDB
