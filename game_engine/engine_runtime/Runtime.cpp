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

        game.onStart(world);

        initializeTiming(TARGET_FPS);

        uint64_t drawDurationUs = 0;
        while (!WindowShouldClose())
        {
            frameData = frameSync();
            inputManager.update();
            game.onUpdateBegin(world);
            world.kinematicsSystem.update(world);
            game.onUpdatePostKinematics(world);
            auto collisions = world.collisionSystem.detect(world);
            game.onUpdatePostCollisionDetection(world, collisions);
            world.collisionSystem.resolve(world, collisions);
            game.onUpdatePostCollisionResolution(world, collisions);
            world.animationSystem.update(world);
            world.cleanup();

            auto drawStart = clock::now();
            renderer.draw(frameData, world, drawDurationUs);
            auto drawEnd = clock::now();
            drawDurationUs =
                std::chrono::duration_cast<std::chrono::microseconds>(drawEnd - drawStart).count();

            if (frameData.workDurationUs > 16'666)
            {
                std::cout
                    << "FPS " << frameData.fps
                    << " | jitter: " << frameData.jitterUs << " us"
                    << " | work: " << frameData.workDurationUs << " us"
                    << " | draw: " << drawDurationUs << " us\n";
            }
        }

        stopTiming();
        CloseWindow();

        return 0;
    }
}
