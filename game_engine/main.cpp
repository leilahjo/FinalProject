#include <chrono>
#include <iostream>

#include "GameObject.h"
#include "InputManager.h"
#include "raylib.h"
#include "Renderer.h"
#include "timing.h"

/*
 * cursor park
 * 🦊        🐢      🦝
 *     🐻‍❄️      🦦
 *   🦉        🐶   🐱
 *        🦒
 * 🐙            🦌  🦙
 *     🐧    🦔
 * 🦦            🐘
 */

GameObject player;
InputManager inputManager;

int main()
{
    InitWindow(800, 600, "Raylib + Console");

    initializeTiming();

    const float frameDt = 1.0 / 60;

    while (!WindowShouldClose())
    {
        FrameData frameData = frameSync();

        inputManager.Update();

        if (inputManager.moveLeft)
        {
            player.x -= player.speed * frameDt;
        }
        if (inputManager.moveRight)
        {
            player.x += player.speed * frameDt;
        }
        if (inputManager.moveUp)
        {
            player.y += player.speed * frameDt;
        }
        if (inputManager.moveDown)
        {
            player.y -= player.speed * frameDt;
        }

        Renderer::DrawFrame(frameData, player);

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
