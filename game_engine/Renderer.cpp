//
// Created by greg8 on 1/27/2026.
//

#include "Renderer.h"

#include <cmath>
#include <iostream>

#include "raylib.h"
#include "Entity.h"

namespace GameEngine
{
    void DrawEntity(Entity& entity, Entity& player);

    void Renderer::DrawFrame(FrameData& frameData, Game::Game& game)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Entity player = game.world.findEntity(game.playerId).value();
        game.world.forEach(Archetype::COMP_POSITION | Archetype::COMP_SIZE | Archetype::COMP_COLOR,
                           [&player](Entity entity)
                           {
                               if (entity != player)
                                   DrawEntity(entity, player);
                           });

        DrawEntity(player, player);

        DrawText(TextFormat("FPS %f | Jitter: %lld us | Work: %lld us", frameData.fps, frameData.jitterUs,
                            frameData.workDurationUs), 0, 0, 14, DARKGRAY);

        EndDrawing();
    }

    void DrawEntity(Entity& entity, Entity& player)
    {
        double windowWidthPx = GetScreenWidth();
        double windowHeightPx = GetScreenHeight();

        double minDim = std::min(windowWidthPx, windowHeightPx);
        double pxPerGameUnit = minDim / 2;

        double originXPx = windowWidthPx / 2;
        double originYPx = windowHeightPx / 2;

        double objXPx = originXPx + (entity.x() - player.x()) * pxPerGameUnit;
        double objYPx = originYPx - (entity.y() - player.y()) * pxPerGameUnit;

        double objWidthPx = entity.width() * pxPerGameUnit;
        double objHeightPx = entity.height() * pxPerGameUnit;

        DrawRectangle(static_cast<int>(std::round(objXPx - objWidthPx / 2)),
                      static_cast<int>(std::round(objYPx - objHeightPx / 2)),
                      static_cast<int>(std::round(objWidthPx)),
                      static_cast<int>(std::round(objHeightPx)),
                      entity.color());
    }
}
