//
// Created by greg8 on 2/21/2026.
//

#include "Runtime.h"

#include <chrono>
#include <iostream>

#include "raylib.h"
#include "util/timing.h"

namespace EngineRuntime
{
    using clock = std::chrono::steady_clock;

    int Runtime::run(IGame& game, int windowWidth, int windowHeight, const std::string& windowTitle)
    {
        InitWindow(windowWidth, windowHeight, windowTitle.c_str());
        audioManager.initialize();

        game.onStart(world);

        initializeTiming(TARGET_FPS);

        uint64_t lastFrameDrawDurationUs = 0;
        while (!WindowShouldClose())
        {
            frameData = frameSync();
            inputManager.update();
            game.onUpdateBegin(world);
            world.kinematicsSystem.update(world);
            game.onUpdatePostKinematics(world);
            size_t collisionCount = 0;
            auto collisions = world.collisionSystem.detect(world, collisionCount, frameAllocator);
            game.onUpdatePostCollisionDetection(world, collisions, collisionCount);
            world.collisionSystem.resolve(world, collisions, collisionCount);
            game.onUpdatePostCollisionResolution(world, collisions, collisionCount);
            world.animationSystem.update(world);
            world.cleanup();

            auto drawStart = clock::now();
            renderer.draw(frameData, world, lastFrameDrawDurationUs);
            auto drawEnd = clock::now();

            frameAllocator.resetFully();

            if (frameData.workDurationUs > 16'666)
            {
                std::cout
                    << "FPS " << frameData.fps
                    << " | jitter: " << frameData.jitterUs << " us"
                    << " | work: " << frameData.workDurationUs << " us"
                    << " | draw: " << lastFrameDrawDurationUs << " us\n";
            }

            lastFrameDrawDurationUs =
                std::chrono::duration_cast<std::chrono::microseconds>(drawEnd - drawStart).count();
        }

        stopTiming();
        CloseWindow();

        return 0;
    }
}
