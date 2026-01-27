//
// Created by greg8 on 1/27/2026.
//

#include "Renderer.h"

#include <cmath>
#include <iostream>

#include "raylib.h"

void DrawGameObject(GameObject gameObject);

void Renderer::DrawFrame(const FrameData& frameData, const GameObject& gameObject)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawGameObject(gameObject);

    DrawText(TextFormat("FPS %f | Jitter: %lld us | Work: %lld us",
                        frameData.fps, frameData.jitterUs, frameData.workDurationUs),
             0, 0, 30, DARKGRAY);
    EndDrawing();
}

void DrawGameObject(GameObject gameObject)
{
    double windowWidthPx = GetScreenWidth();
    double windowHeightPx = GetScreenHeight();

    double minDim = std::min(windowWidthPx, windowHeightPx);
    double pxPerGameUnit = minDim / 2;

    double originXPx = windowWidthPx / 2;
    double originYPx = windowHeightPx / 2;

    double objXPx = originXPx + gameObject.x * pxPerGameUnit;
    double objYPx = originYPx - gameObject.y * pxPerGameUnit;

    double objWidthPx = gameObject.width * pxPerGameUnit;
    double objHeightPx = gameObject.height * pxPerGameUnit;

    DrawRectangle( static_cast<int>(std::round(objXPx - objWidthPx / 2)),
                  static_cast<int>(std::round(objYPx - objHeightPx / 2)),
                  static_cast<int>(std::round(objWidthPx)),
                  static_cast<int>(std::round(objHeightPx)),
                  GREEN);
}
