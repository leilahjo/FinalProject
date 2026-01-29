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

	// Double-unit "border" squares
    game.objects.push_back({0, 0, 0, 0, 2, 2, 0, 0, BLUE});
    game.objects.push_back({0, 0, 0, 0, 1.9, 1.9, 0, 0, RAYWHITE});

	// Bouncing cicrles
    game.objects.push_back({-0.5, -0.5, 0, 0, 0, 0, 0.1, 0.1, GOLD, false});
    game.objects.push_back({-0.25, -0.25, 0, 0, 0, 0, 0.1, 0.1, MAROON, false});
    game.objects.push_back({0.25, 0.25, 0, 0, 0, 0, 0.1, 0.1, PURPLE, false});
    game.objects.push_back({0.5, 0.5, 0, 0, 0, 0, 0.1, 0.1, BEIGE, false});
    
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
