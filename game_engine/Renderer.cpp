//
// Created by greg8 on 1/27/2026.
//

#include "Renderer.h"

#include <cmath>
#include <iostream>

#include "raylib.h"
#include "GameObject.h"

namespace GameEngine
{
    void DrawGameObject(GameObject& gameObject, GameObject& player);

    void Renderer::DrawFrame(FrameData& frameData, Game::Game& game)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < game.objects.size(); ++i)
            DrawGameObject(game.objects[i], game.player);
        DrawGameObject(game.player, game.player);

        DrawText(TextFormat("FPS %f | Jitter: %lld us | Work: %lld us", frameData.fps, frameData.jitterUs,
                            frameData.workDurationUs), 0, 0, 14, DARKGRAY);

        EndDrawing();
    }

    void DrawGameObject(GameObject& gameObject, GameObject& player)
    {
        double windowWidthPx = GetScreenWidth();
        double windowHeightPx = GetScreenHeight();

        double minDim = std::min(windowWidthPx, windowHeightPx);
        double pxPerGameUnit = minDim / 2;

        double originXPx = windowWidthPx / 2;
        double originYPx = windowHeightPx / 2;

        double objXPx = originXPx + (gameObject.x - player.x) * pxPerGameUnit;
        double objYPx = originYPx - (gameObject.y - player.y) * pxPerGameUnit;

        if (gameObject.isRect)
        {
            double objWidthPx = gameObject.width * pxPerGameUnit;
            double objHeightPx = gameObject.height * pxPerGameUnit;

            DrawRectangle(static_cast<int>(std::round(objXPx - objWidthPx / 2)),
                          static_cast<int>(std::round(objYPx - objHeightPx / 2)),
                          static_cast<int>(std::round(objWidthPx)),
                          static_cast<int>(std::round(objHeightPx)),
                          gameObject.color);
        }
        else
        {
            double objRadiusPx = gameObject.radius * pxPerGameUnit;

            DrawCircle(static_cast<int>(std::round(objXPx)),
                       static_cast<int>(std::round(objYPx)),
                       static_cast<float>(objRadiusPx),
                       gameObject.color);
        }
    }
}
