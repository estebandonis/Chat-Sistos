cmake_minimum_required(VERSION 3.23)

project(SERVER)

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


# Include source files
add_executable(SERVER src/server.cpp src/chat.pb.cc)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    ${PROTOBUF_LIBRARIES}
    absl::log_internal_message
    absl::log_internal_check_op
)
