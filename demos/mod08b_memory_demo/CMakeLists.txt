cmake_minimum_required(VERSION 4.0)
project(w3_memory_demo)

set(CMAKE_CXX_STANDARD 20)

add_executable(w3_memory_demo main.cpp
        ArenaAllocator.h)
