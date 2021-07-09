# https://llvm.org/docs/CMake.html#options-and-variables
# # INITIALIZE
cmake_minimum_required(VERSION 3.15)
project(weasel VERSION 0.0.1)

set (WeaselCmake_DIR ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_LINKER ld.lld)
set(LLVM_DIR /usr/lib/llvm-11/lib/cmake/llvm)
set(THIRD_PARTY_DIR third-party)

find_package(LLVM REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})

include(HandleLLVMOptions)
include(AddLLVM)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS ${LLVM_INCLUDE_DIRS})
message(STATUS ${LLVM_DEFINITIONS})

# LLVM Definitions
add_definitions(${LLVM_DEFINITIONS})

# LLVM LIBRARY
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${THIRD_PARTY_DIR}/include)
include_directories(include)

# Weasel Directories
add_subdirectory(tools)
add_subdirectory(lib)
add_subdirectory(libparallel)
add_subdirectory(unittests)
