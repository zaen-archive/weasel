# # INITIALIZE
cmake_minimum_required(VERSION 3.15)
project(underrated VERSION 1.0.0)

set(LLVM_DIR D:/LLVM/lib/cmake/llvm)
set(ZERO_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ZERO_LIBS_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(ZERO_TOOLS_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/tools)

find_package(LLVM REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

include(HandleLLVMOptions)
include(AddLLVM)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS ${LLVM_INCLUDE_DIRS})
message(STATUS ${LLVM_DEFINITIONS})

# LLVM LIBRARY
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# ZERO LIBRARY
include_directories(${ZERO_INCLUDE_DIRS})
add_subdirectory(${ZERO_TOOLS_DIRS})
# add_subdirectory(${ZERO_LIBS_DIRS})

# ADD LLVM LIBS
llvm_map_components_to_libnames(llvm_libs core)

# Show all include directory
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir = '${dir}'")
endforeach()
