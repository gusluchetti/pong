cmake_minimum_required(VERSION 3.15)
project(pong)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-march=native -Wall -Wextra -Wpedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILE main.cpp)

if(WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target x86_64-w64-mingw32 -static")
endif()

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -O0")

add_executable(pong ${SOURCE_FILE})
