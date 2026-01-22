#include <chrono>
#include <iostream>

#include "raylib.h"
#include "timing.h"

int main()
{
    InitWindow(800, 600, "Raylib + Console");

    initializeTiming();

    while (!WindowShouldClose())
    {
        FrameData frameData = frameSync();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(TextFormat("FPS %f | Jitter: %lld us | Work: %lld us", frameData.fps, frameData.jitterUs,
                            frameData.workDurationUs), 100, 100, 30, DARKGRAY);
        EndDrawing();
        //if (frameData.workDurationUs > 16'666)
        {
            std::cout << "FPS " << frameData.fps << " | jitter: " << frameData.jitterUs << " us | work : " << frameData.
                workDurationUs << "us\n";
        }
    }

    stopTiming();

    CloseWindow();

    return 0;
}