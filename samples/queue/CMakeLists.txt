cmake_minimum_required(VERSION 2.8)
project(sample)

include_directories(../llvm-build/Release+Asserts/lib/clang/3.3/include/)
include_directories(/usr/include/)
include_directories(/usr/include/i386-linux-gnu/c++/4.8/)
add_executable(sample queue.cpp)
