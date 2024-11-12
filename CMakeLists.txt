cmake_minimum_required(VERSION 3.15)
project(pong)

set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILE "src/main.cpp")

include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)

# SDL2
find_package(SDL2 QUIET)
if (NOT SDL2_FOUND)
        FetchContent_Declare(
                SDL2
                GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                GIT_TAG release-2.30.9
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE
        )
        FetchContent_MakeAvailable(SDL2)
endif()

# GLEW
find_package(GLEW QUIET)
if (NOT GLEW_FOUND)
        FetchContent_Declare(
                GLEW 
                GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
                GIT_TAG glew-cmake-2.2.0
        )
        FetchContent_MakeAvailable(GLEW)
endif()

add_executable(pong ${SOURCE_FILE})

# GLM
find_package(GLM QUIET)
if (NOT GLM_FOUND)
        FetchContent_Declare(
                GLM
                GIT_REPOSITORY https://github.com/g-truc/glm.git
                GIT_TAG 1.0.1
        )
        FetchContent_MakeAvailable(GLM)
endif()

if(WIN32)
    target_link_libraries(pong PRIVATE 
        SDL2::SDL2-static 
        SDL2::SDL2main
        libglew_static
        opengl32
        glm
    )
    
    # Force static linking for Windows
    set_target_properties(pong PROPERTIES
        LINK_FLAGS "-static -static-libgcc -static-libstdc++"
    )
else()
    target_link_libraries(pong PRIVATE 
        SDL2 
        GLEW 
        GLU 
        GL
        glm
    )
endif()
