cmake_minimum_required(VERSION 3.5)
project(arm11_32)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(SOURCE_FILES
    src/assemble.c)

add_executable(arm11_32 ${SOURCE_FILES})