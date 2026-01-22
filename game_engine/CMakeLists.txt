cmake_minimum_required(VERSION 3.20)
project(game_engine)

include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(game_engine main.cpp
        timing.cpp
        timing.h)
target_link_libraries(game_engine PRIVATE raylib)
target_include_directories(game_engine PRIVATE ${RAYLIB_ROOT}/src)

if(WIN32)
    target_link_libraries(game_engine PRIVATE winmm)
endif()
