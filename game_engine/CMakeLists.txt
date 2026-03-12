cmake_minimum_required(VERSION 3.20)
project(game_engine)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==========================================
# Game Executable
# ==========================================
add_executable(game_engine
        game/main.cpp
        game/Game.cpp
        game/Game.h
)

# ==========================================
# Engine Runtime
# ==========================================
add_library(engine_runtime STATIC
        engine_runtime/EngineRuntime.h
        engine_runtime/Runtime.h
        engine_runtime/util/timing.cpp
        engine_runtime/util/timing.h
        engine_runtime/IGame.h
        engine_runtime/Runtime.cpp
)
target_include_directories(engine_runtime PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(engine_runtime PRIVATE ${RAYLIB_ROOT}/src)

# ==========================================
# Rendering
# ==========================================
add_library(rendering STATIC
        rendering/Renderer.cpp
        rendering/Renderer.h
        rendering/Sprite.cpp
        rendering/Sprite.h
        rendering/Rendering.h
        rendering/SpriteManager.cpp
        rendering/SpriteManager.h
)
target_include_directories(rendering PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(rendering PRIVATE ${RAYLIB_ROOT}/src)

# ==========================================
# Input
# ==========================================
add_library(input STATIC
        input/InputManager.h
        input/InputManager.cpp
)
target_include_directories(input PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(input PRIVATE ${RAYLIB_ROOT}/src)

# ==========================================
# Engine Core
# ==========================================
add_library(engine_core STATIC
        engine_core/ecs/Archetype.cpp
        engine_core/ecs/Archetype.h
        engine_core/ecs/AnimationData.h
        engine_core/ecs/GameColor.h
        engine_core/ecs/ECSTypes.h
        engine_core/ecs/Entity.cpp
        engine_core/ecs/Entity.h
        engine_core/systems/AnimationSystem.cpp
        engine_core/systems/AnimationSystem.h
        engine_core/systems/CollisionSystem.cpp
        engine_core/systems/CollisionSystem.h
        engine_core/systems/KinematicsSystem.cpp
        engine_core/systems/KinematicsSystem.h
        engine_core/util/Random.cpp
        engine_core/util/Random.h
        engine_core/EngineCore.h
        engine_core/World.cpp
        engine_core/World.h
        engine_core/FrameData.h
        engine_core/memory_management/ArenaAllocator.h
)
target_include_directories(engine_core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_link_libraries(game_engine PRIVATE engine_runtime)

target_link_libraries(engine_runtime PUBLIC engine_core)
target_link_libraries(engine_runtime PUBLIC rendering)
target_link_libraries(engine_runtime PUBLIC input)
target_link_libraries(engine_runtime PRIVATE raylib)
target_include_directories(engine_runtime PRIVATE ${RAYLIB_ROOT}/src)

target_link_libraries(rendering PUBLIC engine_core)
target_link_libraries(rendering PUBLIC raylib)
target_include_directories(rendering PRIVATE ${RAYLIB_ROOT}/src)

target_link_libraries(input PRIVATE raylib)

if (WIN32)
    target_link_libraries(game_engine PRIVATE winmm)
endif ()

add_compile_definitions($<$<CONFIG:Debug>:_GLIBCXX_ASSERTIONS>)

add_custom_command(
        TARGET game_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:game_engine>/assets
)