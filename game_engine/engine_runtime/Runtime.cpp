#include "Runtime.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "raylib.h"
#include "engine_core/EngineCore.h"
#include "util/timing.h"

namespace
{
    std::atomic<bool> engineRunning{true};
}

namespace EngineRuntime
{
    using clock = std::chrono::steady_clock;

    int Runtime::run(IGame& game, int winW, int winH, const std::string& title,
                     std::optional<uint32_t> benchmarkFrameLimit)
    {
        if (benchmarkFrameLimit.has_value())
            setSeed(0);

        BlockTimer init("Initialization");
        init.startBlock("Window");
        InitWindow(winW, winH, title.c_str());

        init.startBlock("Sprites");
        renderer.spriteManager.initialize(128);
        init.startBlock("Audio");
        audioManager.initialize(128);
        init.startBlock("Behavior");
        behaviorManager.initialize();
        init.startBlock("Game");
        game.onStart(world, renderer);

        std::atomic<bool> stopFlag{false};
        init.startBlock("Thread");
        std::thread engineThread([&]()
        {
            runEngineLoop(stopFlag, game, benchmarkFrameLimit);
        });

        init.endFrame();
        init.printStats();

        BlockTimer renderTimer("Render loop");
        uint64_t accDraw = 0, drawUs = 0, frames = 0;

        while (!WindowShouldClose() && engineRunning)
        {
            renderTimer.startBlock("await");
            RenderFrame* readFrame = renderer.swapBuffer.awaitReadFrameAndSwap();

            auto t0 = clock::now();
            renderer.draw(drawUs, *readFrame, renderTimer);
            renderTimer.startBlock("input");
            inputManager.update(readFrame->viewportX, readFrame->viewportY,
                                readFrame->viewportScale);
            renderTimer.startBlock("cleanup");
            auto t1 = clock::now();

            drawUs = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
            accDraw += drawUs;
            if (drawUs > 16'666)
                std::cout << "Draw: " << drawUs << " us\n";

            renderTimer.endFrame();
            frames++;
        }

        renderTimer.printStats();

        stopFlag.store(true);
        engineThread.join();

        CloseWindow();

        std::cout << "Render frames: " << frames << "\n";
        if (frames > 0)
            std::cout << "Avg draw: " << accDraw / frames << " us\n";
        return 0;
    }

    uint64_t Runtime::runEngineLoop(std::atomic<bool>& stop, IGame& game,
                                    std::optional<uint32_t> benchmarkFrameLimit)
    {
        ArenaAllocator frameAlloc(10 * 1024 * 1024);
        uint64_t frameCount = 0, accWork = 0;

        BlockTimer loop("Engine loop");
        initializeTiming(TARGET_FPS);

        while (!stop.load())
        {
            loop.startBlock("frameSync");
            auto fd = frameSync(true);
            accWork += fd.workDurationUs;
            if (fd.workDurationUs > 16'666)
                std::cout << "FPS " << fd.fps << " work " << fd.workDurationUs << " us\n";

            loop.startBlock("game.onUpdateBegin");
            game.onUpdateBegin(world);

            loop.startBlock("kinematics");
            world.kinematicsSystem.update(world);

            loop.startBlock("game.onUpdatePostKinematics");
            game.onUpdatePostKinematics(world);

            loop.startBlock("collision.detect");
            size_t collCount = 0;
            auto* collisions = world.collisionSystem.detect(world, collCount, frameAlloc);

            loop.startBlock("game.onUpdatePostCollisionDetection");
            game.onUpdatePostCollisionDetection(world, collisions, collCount);

            loop.startBlock("collision.resolve");
            world.collisionSystem.resolve(world, collisions, collCount);

            loop.startBlock("game.onUpdatePostCollisionResolution");
            game.onUpdatePostCollisionResolution(world, collisions, collCount);

            loop.startBlock("animation");
            world.animationSystem.update(world);

            loop.startBlock("hpCheck");
            world.hpCheckSystem.update(world);

            loop.startBlock("particles");
            world.particleSystem.update(world);

            if (inputManager.keyF5Pressed)
            {
                loop.startBlock("behaviorReload");
                behaviorManager.reloadAllBehaviors();
            }
            behaviorManager.update(world, loop);

            if (inputManager.keyF1Pressed)
                renderer.debugRenderEnabled = !renderer.debugRenderEnabled;

            loop.startBlock("world.cleanup");
            world.cleanup();

            audioManager.updateMusic(world.frameDt);

            loop.startBlock("renderer.frameSync");
            auto* writeFrame = renderer.swapBuffer.getWriteFrame();
            renderer.frameSync(fd, world, *writeFrame);
            renderer.swapBuffer.swapWriteFrame();

            loop.startBlock("cleanup");
            frameAlloc.resetFully();
            frameCount++;

            loop.endFrame();
            if (benchmarkFrameLimit.has_value() && frameCount >= benchmarkFrameLimit.value())
                break;
        }

        loop.printStats();
        engineRunning = false;
        renderer.swapBuffer.swapWriteFrame();
        stopTiming();

        std::cout << "Engine frames: " << frameCount << "\n";
        if (frameCount > 0)
            std::cout << "Avg work: " << accWork / frameCount << " us\n";
        return frameCount;
    }
}
