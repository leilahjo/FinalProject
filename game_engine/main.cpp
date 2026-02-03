#include <chrono>
#include <iostream>

#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "raylib.h"
#include "Renderer.h"
#include "timing.h"

using namespace GameEngine;

InputManager inputManager;
Game::Game game;

int main()
{
    InitWindow(800, 600, "Raylib + Console");

    initializeTiming();
    
    while (!WindowShouldClose())
    {
        FrameData frameData = frameSync();
        // Sometimes "realDt" is useful, but not for world state updates!
        // We want consistency, predictability, and stability.
        // const float realDt = 1 / frameData.fps;

        inputManager.Update();
        game.Update(inputManager);
        Renderer::DrawFrame(frameData, game);

        if (frameData.workDurationUs > 16'666)
        {
            std::cout << "FPS " << frameData.fps << " | jitter: "
                << frameData.jitterUs << " us | work : " << frameData.
                workDurationUs << "us\n";
        }
    }

    stopTiming();

    CloseWindow();

    return 0;
}
