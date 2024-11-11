cmake_minimum_required(VERSION 3.15)
project(pong)

set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 QUIET)
if (NOT SDL2_FOUND)
        include(FetchContent)
        Set(FETCHCONTENT_QUIET FALSE)

        FetchContent_Declare(
                SDL2
                GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                GIT_TAG release-2.30.9
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE
        )
        FetchContent_MakeAvailable(SDL2)
endif()
find_package(OpenGL REQUIRED)
set(SOURCE_FILE main.cpp)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -O0")

add_library(glad STATIC "external/glad/src/glad.c")
include_directories("external/glad/include")

add_executable(pong ${SOURCE_FILE})
target_link_libraries(pong SDL2::SDL2main SDL2::SDL2-static OpenGL::GL glad)
