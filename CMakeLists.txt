cmake_minimum_required(VERSION 3.15)
project(pong_cpp)

# Compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-march=native -Wall -Wextra -Wpedantic")

# Source file
set(SOURCE_FILE main.cpp)

# Add Windows-specific flags
if(WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target x86_64-w64-mingw32 -static")
    set(OUTPUT_NAME "a.exe")
else()
    set(OUTPUT_NAME "a.out")
endif()

# Define flags for different build types
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -O0")

# Create the executable
add_executable(${OUTPUT_NAME} ${SOURCE_FILE})
