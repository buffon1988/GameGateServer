cmake_minimum_required(VERSION 3.5)
project(GateServer CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output
set(EXECUTABLE_NAME "GateServer")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Compiler flags
set(CXX_FLAGS
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Woverloaded-virtual
    -Wpointer-arith
    -Wwrite-strings
    -std=c++11
    -rdynamic
    )

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CXX_FLAGS "${CXX_FLAGS} -g -O0")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CXX_FLAGS "${CXX_FLAGS} -O2 -DNDEBUG")
endif()

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")

# --- Include Paths ---
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdParty/muduo/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdParty/msgpack/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdParty/openssl/include)

# --- Third-Party Libraries ---
set(MUDUO_LIB_DIR "${PROJECT_SOURCE_DIR}/3rdParty/muduo/lib")
set(OPENSSL_LIB_DIR "${PROJECT_SOURCE_DIR}/3rdParty/openssl/lib")

find_library(MUDUO_NET libmuduo_net.a ${MUDUO_LIB_DIR})
find_library(MUDUO_BASE libmuduo_base.a ${MUDUO_LIB_DIR})
find_library(CRYPTO libcrypto.a ${OPENSSL_LIB_DIR})

message(STATUS "muduo_net: ${MUDUO_NET}")
message(STATUS "muduo_base: ${MUDUO_BASE}")
message(STATUS "crypto: ${CRYPTO}")

# --- Source Files ---
# GateServer module
aux_source_directory(${PROJECT_SOURCE_DIR}/GateServer SRC_GATESERVER)

# Base library (minimal set for GateServer)
set(BASE_SRCS
    ${PROJECT_SOURCE_DIR}/include/BaseFunc.cpp
    ${PROJECT_SOURCE_DIR}/include/BufReadWrite.cpp
    ${PROJECT_SOURCE_DIR}/include/IniConfig.cpp
    ${PROJECT_SOURCE_DIR}/include/LMsg.cpp
    ${PROJECT_SOURCE_DIR}/include/LUser.cpp
    ${PROJECT_SOURCE_DIR}/include/LVideo.cpp
    ${PROJECT_SOURCE_DIR}/include/ClientNet.cpp
    ${PROJECT_SOURCE_DIR}/include/ServerNet.cpp
)

set(SRC_LIST ${SRC_GATESERVER} ${BASE_SRCS})

# --- Executable ---
add_executable(${EXECUTABLE_NAME} ${SRC_LIST})

# --- Link Libraries ---
target_link_libraries(${EXECUTABLE_NAME}
    ${MUDUO_NET}
    ${MUDUO_BASE}
    ${CRYPTO}
    pthread
    rt
    dl
)

# --- Install ---
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
install(DIRECTORY config/ DESTINATION config)
