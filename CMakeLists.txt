cmake_minimum_required(VERSION 3.1)

project(CHAT-SISTOS)

# Enable C++20 features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_OSX_ARCHITECTURES arm64)

# Find Protobuf library (adjust path if necessary)
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

# Find Abseil library (adjust path if necessary)
find_package(absl REQUIRED)
include_directories(${absl_INCLUDE_DIRS})

# Protocol source files
add_library(protocol src/protocol/chat.pb.cc src/protocol/message.cpp)

target_include_directories(protocol
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(protocol
    ${PROTOBUF_LIBRARIES}
    # absl::log_internal_message
    # absl::log_internal_check_op
)

# Server source files
add_executable(server src/server.cpp)

target_include_directories(server
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(server
    protocol
)

# Client source files
add_executable(client src/client.cpp)

target_include_directories(client
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(client
    protocol
)
