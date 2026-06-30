# GateServer

一个面向游戏服务器的生产级 TCP 网关/代理服务，基于 **muduo** C++ 网络库构建。

## 概述

GateServer 是一个轻量级、高性能的网关服务，运行在游戏客户端与后端逻辑服务器之间。它负责处理客户端连接、消息路由、会话管理和安全验证——作为所有客户端流量的统一入口。

### 核心特性

- **双向消息路由**：根据玩家状态智能路由（大厅 → LogicManager，牌桌 → LogicServer）
- **会话管理**：基于 UUID + 随机密钥 + 消息序列号的防重放认证
- **安全性**：每条客户端消息均附带 MD5 签名验证
- **动态后端发现**：通过 LogicManager 广播自动发现 LogicServer 实例
- **心跳监控**：每 30 秒向所有后端连接发送心跳，支持自动重连
- **空闲检测**：自动清理未认证的连接和不活跃的用户
- **广播支持**：可将推广消息（如跑马灯）转发给所有已连接的客户端

## 架构

```
                    ┌─────────────┐
                    │   客户端    │ (手机/PC 游戏客户端)
                    └──────┬──────┘
                           │ TCP
                    ┌──────▼──────┐
                    │  GateServer │
                    │  (本项目)    │
                    └──────┬──────┘
                           │
              ┌────────────┼────────────┐
              │            │            │
     ┌────────▼──┐  ┌──────▼──────┐ ┌───▼────────┐
     │LogicServer│  │LogicServer  │ │ ...        │  (游戏逻辑服务器)
     └───────────┘  └─────────────┘ └────────────┘
              ▲
              │
     ┌────────▼──────────┐
     │  LogicManager     │  (后端编排器)
     └───────────────────┘
```

### 线程模型

| 线程 | 数量 | 用途 |
|------|------|------|
| 主 IO 线程 | 1 | 接受客户端连接，运行 muduo EventLoop |
| 子 IO 线程 | 4 | 处理客户端 I/O 读写 |
| 后端连接线程 | 1 | 连接 LogicManager 和 LogicServer（自动重连） |
| 业务处理线程 | 1 | 消息处理队列（BlockingQueue → handler 分发） |

## 安装

### 前置条件

- **操作系统**：Linux (Ubuntu 16.04+、CentOS 7+) 或 macOS
- **编译器**：GCC 4.8+ 或支持 C++11 的 Clang
- **CMake**：3.5+
- **系统库**：pthread、rt、dl（通常包含在 libc 中）

### 编译

```bash
# 克隆仓库
git clone <your-repo-url>
cd GateServer

# 创建构建目录
mkdir build && cd build

# 配置并编译
cmake ..
make -j$(nproc)

# 可执行文件位于：bin/GateServer
```

> **注意**：第三方库（muduo、OpenSSL）的静态库是为 **Linux** 预编译的。如果你在 macOS 上使用，可能需要重新编译 muduo 或交叉编译。

### 第三方依赖

所有依赖均已内置在 `3rdParty/` 目录中：

| 库 | 用途 |
|----|------|
| **muduo** | C++ 异步网络库（Netty 风格） |
| **msgpack** | 消息协议的二进制序列化 |
| **OpenSSL** | 用于消息完整性的 MD5 哈希 |

无需在系统层面安装任何库——所有库均为预编译的静态归档文件。

## 配置

复制示例配置文件并进行编辑：

```bash
cp config/gateserver_config.ini.example config/gateserver_config.ini
```

### 配置项说明

```ini
# 客户端连接的 TCP 监听端口
ListenPort = 8115

# GateServer 标识
GateServerId = 1

# GateServer 自身 IP（向后端注册时使用）
GateServerIp = 127.0.0.1

# 后端连接的认证密钥
GateServerKey = change_me_to_a_secure_key

# LogicManager 连接信息
LogicManagerIp = 127.0.0.1
LogicManagerPort = 11015
```

## 快速开始

### 1. 编译

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 2. 配置

复制示例配置文件，并根据实际环境至少修改 `LogicManagerIp` / `LogicManagerPort`：

```bash
cp config/gateserver_config.ini.example config/gateserver_config.ini
# 编辑 config/gateserver_config.ini
```

### 3. 运行

```bash
# 从项目根目录运行（确保 config/ 目录可访问）
./build/bin/GateServer

# 或者进入 bin 目录运行
cd build/bin
./GateServer
```

### 4. 验证

查看日志输出确认服务正常启动：
- `GateServer registered with LogicManager` — 后端连接已建立
- `LogicServer list updated` — 已发现后端服务器
- 配置的 `ListenPort` 上有客户端连接日志

### 配置文件查找顺序

GateServer 按以下顺序搜索配置文件：

1. `./config/gateserver_config.ini`（相对于工作目录）
2. `config/gateserver_config.ini`
3. `../config/gateserver_config.ini`
4. 若均未找到，则回退到硬编码的默认值

## 使用方法

已完成编译和配置后，从项目根目录运行：

```bash
# 从项目根目录运行（确保 config/ 目录可访问）
./build/bin/GateServer

# 或者进入 bin 目录
cd build/bin
./GateServer
```

## 协议

### 线框格式

```
|-- 2字节 bodyLen --|-- 2字节 checksum --|-- 4字节 timestamp --|-- 4字节 msgId --|
|------------------------------------------------------------------------------|--- N字节 body ---|
```

### 客户端消息格式

```
|-- 2字节 verifyHeadLen --|-- msgpack verifyHead --|-- 2字节 entityLen --|-- 1字节 packed --|-- msgpack entity --|
```

verifyHead 包含以下字段：

- `m_msgId`：消息类型 ID
- `m_strUserId`：玩家 UUID
- `m_strVerifyKey`：每次会话的认证密钥
- `m_lMsgOrder`：单调递增的序列号
- `m_iMd5Begin/End`：MD5 验证的字节范围
- `m_strMd5`：body 的 MD5 签名

### 安全措施

1. **MD5 完整性校验**：每条客户端消息体均使用 MD5 签名，服务端使用共享的 `MD5_SIGN` 常量进行验证。
2. **会话密钥**：登录时服务端为每个客户端生成 32 位随机密钥，后续每条消息均需携带该密钥。
3. **防重放攻击**：每条消息携带序列号（`m_lMsgOrder`），序列号 ≤ 上次已见值的消息将被拒绝。
4. **超时机制**：未认证的连接在 30 秒后断开；不活跃的已认证用户也会在 30 秒后被踢出。

## 消息类型

### 客户端 → 服务端

| 消息 ID | 消息名 | 描述 |
|---------|--------|------|
| 170 | `MSG_C_2_S_LOGIN_GATE` | 客户端登录 Gate |
| 171 | `MSG_S_2_C_LOGIN_GATE` | Gate 响应，返回会话密钥 |
| 15 | `MSG_C_2_S_HEART` | 客户端心跳 |
| 16 | `MSG_S_2_C_HEART` | 服务端心跳响应 |
| 180 | `MSG_S_2_C_SHUTDOWN_GATE` | 服务端断开客户端连接 |

### 服务端 ↔ 后端

| 消息 ID | 消息名 | 方向 | 描述 |
|---------|--------|------|------|
| 12001 | `MSG_G_2_LMG_LOGIN` | Gate → LM | Gate 注册 |
| 12010 | `MSG_LMG_2_G_SYNC_LOGIC` | LM → Gate | LogicServer 列表更新 |
| 12011 | `MSG_LMG_2_G_HORSE_INFO` | LM → Gate | 推广消息广播 |
| 8000 | `MSG_G_2_L_USER_MSG` | Gate → Logic | 转发客户端消息 |
| 8001 | `MSG_L_2_G_USER_MSG` | Logic → Gate | 转发至客户端 |
| 8006 | `MSG_L_2_G_MODIFY_USER_STATE` | Logic → Gate | 用户状态变更 |

## 开发

### 添加新的游戏处理器

本 GateServer 设计为可与遵循相同协议的任何游戏后端配合使用。集成步骤如下：

1. 配置 `LogicManagerIp` 和 `LogicManagerPort` 指向你的后端编排器
2. 确保你的后端发送 `MSG_LMG_2_G_SYNC_LOGIC` 消息，其中包含 `LogicInfo` 结构体（含 `m_id`、`m_ip`、`m_port`）
3. 确保你的后端发送 `MSG_LMG_2_G_HORSE_INFO` 用于广播消息
4. GateServer 将自动发现并连接到所有列出的 LogicServer

### 扩展网关

核心消息路由器位于 `GateServer/Business.cpp`。如需添加新功能：

1. 在 `include/CommonDefine.h` 中定义新的消息 ID
2. 在 `include/LMsgSS.h` 中添加消息结构体
3. 在 `include/LMsg.cpp::InitMsg()` 中注册消息
4. 在 `Business::handleMsg()` 中添加对应的 case 以分发新消息类型

## 项目结构

```
├── CMakeLists.txt          # 构建配置
├── README.md               # 英文说明文档
├── README_CN.md            # 中文说明文档（本文件）
├── include/                # 共享基础库（GateServer 所需的最小集）
│   ├── LMsg.h/cpp          # 消息框架与工厂
│   ├── LMsgSS.h            # 服务端间消息定义
│   ├── LMsgCS.h            # 客户端到服务端消息定义
│   ├── ClientNet.h/cpp     # 出站 TCP 客户端封装
│   ├── ServerNet.h/cpp     # 入站 TCP 服务端封装
│   ├── Decode.h            # 协议解析器（msgpack/MD5）
│   ├── BaseFunc.h/cpp      # 工具函数（MD5、随机数、解析）
│   ├── IniConfig.h/cpp     # INI 配置文件解析器
│   ├── CommonDefine.h      # 全局常量和枚举
│   ├── GameDefine.h        # 游戏相关常量
│   └── ...                 # 其他基础组件
├── GateServer/             # GateServer 模块
│   ├── main.cpp            # 入口点
│   ├── Work.h/cpp          # 生命周期编排
│   ├── Business.h/cpp      # 核心消息路由逻辑
│   ├── Config.h/cpp        # 配置加载
│   └── GateUser.h/cpp      # 每客户端会话管理
├── 3rdParty/               # 内置第三方库
│   ├── muduo/              # C++ 异步网络库
│   ├── msgpack/            # 二进制序列化
│   └── openssl/            # 密码学（MD5）
└── config/                 # 配置文件
    └── gateserver_config.ini.example
```

## 许可证

本项目源自 GameServers 项目，采用 MIT 许可证发布。详见原项目。

## 备注

- 这是一个从大型 GameServers 项目中**仅提取的网关模块**
- 完整的 GameServers 项目包含：LoginServer、CenterServer、LogicManager、LogicServer 和 LogicDB
