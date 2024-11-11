cmake_minimum_required(VERSION 3.15)
project(pong)

set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
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

set(SOURCE_FILE main.cpp)

if(WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target x86_64-w64-mingw32 -static")
endif()

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -O0")

include_directories(${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})

add_executable(pong ${SOURCE_FILE})
