cmake_minimum_required(VERSION 3.19)

project(test-protoc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../UserApp/protocols)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../UserApp/SDK)

find_package(Git REQUIRED)

execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)


add_definitions(-DGIT_VERSION="${GIT_VERSION}")
add_definitions(-DGIT_HASH="${GIT_HASH}")
add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")


add_executable(test-protoc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../UserApp/protocols/hid_msg.pb.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../UserApp/protocols/pb_decode.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../UserApp/protocols/pb_encode.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../UserApp/protocols/pb_common.c
        main.cpp)
