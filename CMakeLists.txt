# # INITIALIZE
cmake_minimum_required(VERSION 3.15)
project(underrated VERSION 1.0.0)

set(LLVM_DIR D:/LLVM/lib/cmake/llvm)

find_package(LLVM REQUIRED CONFIG)


list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

include(AddLLVM)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs core)

# add_subdirectory(lib)
add_subdirectory(examples)
# add_subdirectory(tools)
