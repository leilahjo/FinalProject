//
// Created by greg8 on 2/21/2026.
//

#include "Runtime.h"

#include <chrono>
#include <iostream>
#include <stop_token>
#include <thread>

#include "raylib.h"
#include "util/timing.h"

namespace EngineRuntime
{
    using clock = std::chrono::steady_clock;

    int Runtime::run(IGame& game, int windowWidth, int windowHeight, const std::string& windowTitle)
    {
        InitWindow(windowWidth, windowHeight, windowTitle.c_str());

        renderer.spriteManager.initialize(128);
        audioManager.initialize(128);
        behaviorManager.initialize(128);

        game.onStart(world, renderer);

        std::jthread engineThread([this, &game](std::stop_token token)
        {
            runEngineLoop(std::move(token), game);
        });

        uint64_t drawDurationUs = 0;
        uint64_t frameCount = 0;
        while (!WindowShouldClose())
        {
            // The "main" loop is now just the rendering loop
            RenderFrame* readRenderFrame = renderer.swapBuffer.awaitReadFrameAndSwap();
            auto drawStart = clock::now();
            renderer.draw(drawDurationUs, *readRenderFrame);
            inputManager.update();
            auto drawEnd = clock::now();

            drawDurationUs =
                std::chrono::duration_cast<std::chrono::microseconds>(drawEnd - drawStart).count();
            if (drawDurationUs > 16'666)
                std::cout << "Draw duration: " << drawDurationUs << " us\n";
            frameCount++;
        }

        engineThread.request_stop();
        engineThread.join();

        CloseWindow();

        std::cout << "Render frame count: " << frameCount << std::endl;

        return 0;
    }

    uint64_t Runtime::runEngineLoop(std::stop_token token, IGame& game)
    {
        ArenaAllocator frameAllocator = ArenaAllocator(10 * 1024 * 1024);
        uint64_t frameCount = 0;
        initializeTiming(TARGET_FPS);
        while (!token.stop_requested())
        {
            auto frameData = frameSync();
            game.onUpdateBegin(world);
            world.kinematicsSystem.update(world);
            game.onUpdatePostKinematics(world);
            // std::this_thread::sleep_for(std::chrono::milliseconds(250));
            size_t collisionCount = 0;
            auto collisions = world.collisionSystem.detect(world, collisionCount, frameAllocator);
            game.onUpdatePostCollisionDetection(world, collisions, collisionCount);
            world.collisionSystem.resolve(world, collisions, collisionCount);
            game.onUpdatePostCollisionResolution(world, collisions, collisionCount);
            world.animationSystem.update(world);
            if (inputManager.keyF5Pressed)
                behaviorManager.reloadAllBehaviors();
            behaviorManager.update(world);
            world.cleanup();

            auto writeRenderFrame = renderer.swapBuffer.getWriteFrame();
            renderer.frameSync(frameData, world, *writeRenderFrame);
            renderer.swapBuffer.swapWriteFrame();

            frameAllocator.resetFully();

            if (frameData.workDurationUs > 16'666)
            {
                std::cout
                    << "FPS " << frameData.fps
                    << " | jitter: " << frameData.jitterUs << " us"
                    << " | work: " << frameData.workDurationUs << " us"
                    << std::endl;
            }
            frameCount++;
        }

        stopTiming();
        std::cout << "Engine frame count: " << frameCount << std::endl;

        return frameCount;
    }
}
