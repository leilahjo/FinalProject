#include <iostream>

#include "raylib.h"
#include "timing.h"

#include "Game.h"
#include "InputManager.h"
#include "Renderer.h"

Game::Game game;
InputManager inputManager;

int main()
{
    InitWindow(1200, 800, "Raylib + Console");

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
