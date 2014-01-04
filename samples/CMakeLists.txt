cmake_minimum_required(VERSION 2.8)
project(sample)

include_directories(../llvm-build/Release+Asserts/lib/clang/3.3/include/)
include_directories(./)
add_executable(sample main.cpp library.cpp)
