//
// Created by greg8 on 2/21/2026.
//

#include "Runtime.h"

#include <chrono>
#include <iostream>
#include <stop_token>
#include <thread>

#include "raylib.h"
#include "engine_core/EngineCore.h"
#include "util/timing.h"

namespace
{
    volatile bool engineRunning = true;
}

namespace EngineRuntime
{
    using clock = std::chrono::steady_clock;

    int Runtime::run(IGame& game, int windowWidth, int windowHeight, const std::string& windowTitle,
                     std::optional<uint32_t> benchmarkFrameLimit)
    {
        if (benchmarkFrameLimit.has_value())
            setSeed(0);

        BlockTimer blockTimer = BlockTimer("Initialization");

        blockTimer.startBlock("Window");
        InitWindow(windowWidth, windowHeight, windowTitle.c_str());

        blockTimer.startBlock("Sprites");
        renderer.spriteManager.initialize(128);
        blockTimer.startBlock("Audio");
        audioManager.initialize(128);
        blockTimer.startBlock("Behavior");
        behaviorManager.initialize();

        blockTimer.startBlock("Game");
        game.onStart(world, renderer);


        blockTimer.startBlock("Thread");
        std::jthread engineThread([&](std::stop_token token)
        {
            runEngineLoop(std::move(token), game, benchmarkFrameLimit);
        });

        blockTimer.endFrame();
        blockTimer.printStats();

        BlockTimer renderLoopTimer = BlockTimer("Render loop");
        uint64_t accumulatedDrawDurationUs = 0;
        uint64_t drawDurationUs = 0;
        uint64_t frameCount = 0;
        while (!WindowShouldClose() && engineRunning)
        {
            // The "main" loop is now just the rendering loop
            renderLoopTimer.startBlock("await");
            RenderFrame* readRenderFrame = renderer.swapBuffer.awaitReadFrameAndSwap();
            auto drawStart = clock::now();
            renderer.draw(drawDurationUs, *readRenderFrame, renderLoopTimer);
            renderLoopTimer.startBlock("input");
            inputManager.update();
            renderLoopTimer.startBlock("cleanup");
            auto drawEnd = clock::now();

            drawDurationUs =
                std::chrono::duration_cast<std::chrono::microseconds>(drawEnd - drawStart).count();
            accumulatedDrawDurationUs += drawDurationUs;
            if (drawDurationUs > 16'666)
                std::cout << "Draw duration: " << drawDurationUs << " us\n";
            renderLoopTimer.endFrame();
            frameCount++;
        }

        renderLoopTimer.printStats();

        if (engineRunning)
        {
            engineThread.request_stop();
            engineThread.join();
        }

        CloseWindow();

        std::cout << "Render frame count: " << frameCount << std::endl;
        std::cout << "Average draw time: " << accumulatedDrawDurationUs / frameCount << " us" << std::endl;

        return 0;
    }

    uint64_t Runtime::runEngineLoop(std::stop_token token, IGame& game, std::optional<uint32_t> benchmarkFrameLimit)
    {
        ArenaAllocator frameAllocator = ArenaAllocator(10 * 1024 * 1024);
        uint64_t frameCount = 0;
        uint64_t accumulatedWorkTimeUs = 0;

        BlockTimer engineLoopTimer = BlockTimer("Engine loop");

        initializeTiming(TARGET_FPS);
        while (!token.stop_requested())
        {
            engineLoopTimer.startBlock("frameSync");
            auto frameData = frameSync(true);
            accumulatedWorkTimeUs += frameData.workDurationUs;
            if (frameData.workDurationUs > 16'666)
            {
                std::cout
                    << "FPS " << frameData.fps
                    << " | jitter: " << frameData.jitterUs << " us"
                    << " | work: " << frameData.workDurationUs << " us"
                    << std::endl;
            }

            engineLoopTimer.startBlock("game.onUpdateBegin");
            game.onUpdateBegin(world);
            engineLoopTimer.startBlock("kinematicsSystem.update");
            world.kinematicsSystem.update(world);
            engineLoopTimer.startBlock("game.onUpdatePostKinematics");
            game.onUpdatePostKinematics(world);
            // std::this_thread::sleep_for(std::chrono::milliseconds(250));
            engineLoopTimer.startBlock("collisionSystem.detect");
            size_t collisionCount = 0;
            auto collisions = world.collisionSystem.detect(world, collisionCount, frameAllocator);
            engineLoopTimer.startBlock("game.onUpdatePostCollisionDetection");
            game.onUpdatePostCollisionDetection(world, collisions, collisionCount);
            engineLoopTimer.startBlock("collisionSystem.resolve");
            world.collisionSystem.resolve(world, collisions, collisionCount);
            engineLoopTimer.startBlock("game.onUpdatePostCollisionResolution");
            game.onUpdatePostCollisionResolution(world, collisions, collisionCount);
            engineLoopTimer.startBlock("animationSystem.update");
            world.animationSystem.update(world);
            if (inputManager.keyF5Pressed)
            {
                engineLoopTimer.startBlock("behaviorSystem.reloadAllBehaviors");
                behaviorManager.reloadAllBehaviors();
            }
            behaviorManager.update(world, engineLoopTimer);
            engineLoopTimer.startBlock("world.cleanup");
            world.cleanup();

            engineLoopTimer.startBlock("renderer.getWriteFrame");
            auto writeRenderFrame = renderer.swapBuffer.getWriteFrame();
            engineLoopTimer.startBlock("renderer.frameSync");
            renderer.frameSync(frameData, world, *writeRenderFrame);
            engineLoopTimer.startBlock("renderer.swapWriteFrame");
            renderer.swapBuffer.swapWriteFrame();

            engineLoopTimer.startBlock("cleanup");
            frameAllocator.resetFully();
            frameCount++;

            engineLoopTimer.endFrame();
            if (benchmarkFrameLimit.has_value() && frameCount >= benchmarkFrameLimit.value())
                break;
        }

        engineLoopTimer.printStats();

        engineRunning = false;
        renderer.swapBuffer.swapWriteFrame(); // notify the render thread.

        stopTiming();
        std::cout << "Engine frame count: " << frameCount << std::endl;
        std::cout << "Average work time: " << accumulatedWorkTimeUs / frameCount << " us" << std::endl;

        return frameCount;
    }
}
