cmake_minimum_required(VERSION 3.1)

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
#include_directories(${LLVM_INCLUDE_DIRS})
#link_directories(${LLVM_LIBRARY_DIRS})

include_directories("/usr/lib/llvm-3.8/include")
link_directories("/usr/lib/llvm-3.8/lib")

add_subdirectory(skeleton)  # Use your pass name here.
add_subdirectory(count_bb)
