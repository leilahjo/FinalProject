cmake_minimum_required(VERSION 3.20)
project(game_engine)
set(CMAKE_EXE_LINKER_FLAGS "-static") # For external profiling
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# ==========================================
# External Dependencies
# ==========================================
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
        sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        GIT_TAG develop # v3.3.1
)
FetchContent_MakeAvailable(sol2)

# ==========================================
# Fetch official Lua source manually
# ==========================================
set(LUA_VERSION "5.4.6")
set(LUA_TAR "${CMAKE_BINARY_DIR}/lua-${LUA_VERSION}.tar.gz")
set(LUA_SRC_DIR "${CMAKE_BINARY_DIR}/lua-${LUA_VERSION}/src")
# Only download and extract if we haven't already
if(NOT EXISTS "${LUA_SRC_DIR}/lua.h")
    message(STATUS "Downloading and extracting Lua ${LUA_VERSION}...")
    file(DOWNLOAD https://www.lua.org/ftp/lua-${LUA_VERSION}.tar.gz ${LUA_TAR})
    file(ARCHIVE_EXTRACT INPUT ${LUA_TAR} DESTINATION ${CMAKE_BINARY_DIR})
endif()
# Gather the C files, excluding the standalone executables
file(GLOB LUA_SOURCES "${LUA_SRC_DIR}/*.c")
list(REMOVE_ITEM LUA_SOURCES
        "${LUA_SRC_DIR}/lua.c"
        "${LUA_SRC_DIR}/luac.c"
)
add_library(lua STATIC ${LUA_SOURCES})
target_include_directories(lua PUBLIC "${LUA_SRC_DIR}")

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
        engine_core/util/BlockTimer.cpp
        engine_core/util/BlockTimer.h
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
        rendering/RenderProxy.h
        rendering/RenderFrame.h
        rendering/SwapBuffer.h
        rendering/SwapBuffer.cpp
)
target_include_directories(rendering PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(rendering PRIVATE ${RAYLIB_ROOT}/src)

# ==========================================
# Audio
# ==========================================
add_library(audio STATIC
        audio/AudioManager.cpp
        audio/AudioManager.h
        audio/Audio.h
        audio/SoundEffect.cpp
        audio/SoundEffect.h
        audio/AudioTypes.h
)
target_include_directories(audio PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(audio PRIVATE ${RAYLIB_ROOT}/src)

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
# Scripting
# ==========================================
add_library(scripting STATIC
        scripting/BehaviorManager.cpp
        scripting/BehaviorManager.h
        scripting/Scripting.h
        scripting/Behavior.h
        scripting/Behavior.cpp
        scripting/EntityProvider.h
        scripting/Prefab.h
        scripting/PrefabManager.cpp
        scripting/PrefabManager.h
)
target_include_directories(scripting PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

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
target_link_libraries(engine_runtime PUBLIC audio)
target_link_libraries(engine_runtime PUBLIC input)
target_link_libraries(engine_runtime PUBLIC scripting)
target_link_libraries(engine_runtime PRIVATE raylib)
target_include_directories(engine_runtime PRIVATE ${RAYLIB_ROOT}/src)

target_link_libraries(rendering PUBLIC engine_core)
target_link_libraries(rendering PUBLIC raylib)
target_include_directories(rendering PRIVATE ${RAYLIB_ROOT}/src)

target_link_libraries(audio PRIVATE raylib)

target_link_libraries(input PRIVATE raylib)

target_link_libraries(scripting PUBLIC engine_core)
target_link_libraries(scripting PUBLIC sol2)
target_link_libraries(scripting PUBLIC lua)

if (WIN32)
    target_link_libraries(game_engine PRIVATE winmm)
endif ()

add_compile_definitions($<$<CONFIG:Debug>:_GLIBCXX_ASSERTIONS>)

target_compile_definitions(game_engine
        PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

add_custom_command(
        TARGET game_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:game_engine>/assets
)