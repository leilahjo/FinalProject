//
// Created by greg8 on 1/27/2026.
//

#include <cmath>

#include "Renderer.h"
#include "raylib.h"

namespace Rendering
{
    void Renderer::draw(FrameData& frameData, World& world, uint64_t drawDurationUs)
    {
        BeginDrawing();
        // TODO Parameterize
        ClearBackground(::BLACK);

        // TODO type -> render layer map
        world.forEach(Archetype::COMP_POSITION | Archetype::COMP_SIZE,
                      [&](Entity entity)
                      {
                          drawEntity(entity, world);
                      });

        DrawText(TextFormat("FPS %f\nJitter: %lld us\nWork: %lld us\nDraw: %lld us", frameData.fps, frameData.jitterUs,
                            frameData.workDurationUs, drawDurationUs), 0, 0, 36, ::GRAY);

        EndDrawing();
    }

    void Renderer::drawEntity(Entity entity, World& world)
    {
        double windowWidthPx = GetScreenWidth();
        double windowHeightPx = GetScreenHeight();

        double minDim = std::min(windowWidthPx, windowHeightPx);
        double pxPerGameUnit = minDim / 2;

        double originXPx = windowWidthPx / 2;
        double originYPx = windowHeightPx / 2;

        double objXPx = originXPx + (entity.x() - viewPointX) * pxPerGameUnit;
        double objYPx = originYPx - (entity.y() - viewPointY) * pxPerGameUnit;

        double objWidthPx = entity.width() * pxPerGameUnit;
        double objHeightPx = entity.height() * pxPerGameUnit;


        Animation* animation = entity.hasAnimation()
                                   ? world.animationSystem.getAnimation(entity.animation().animationId)
                                   : nullptr;
        Sprite* sprite = entity.hasSprite()
                             ? spriteManager.getSprite(entity.spriteId())
                             : nullptr;

        if (sprite)
        {
            Rectangle srcRect;
            if (animation)
                srcRect = sprite->GetSourceRect(animation->getCurrentFrameIndex(entity.animation().progress));
            else
                srcRect = sprite->GetSourceRect();
            DrawTexturePro(sprite->texture2D, srcRect, Rectangle{
                               static_cast<float>(objXPx - objWidthPx / 2),
                               static_cast<float>(objYPx - objHeightPx / 2),
                               static_cast<float>(objWidthPx),
                               static_cast<float>(objHeightPx)
                           },
                           {},
                           0,
                           entity.hasColor() ? std::bit_cast<::Color>(entity.color()) : WHITE);
        }
        else
        {
            DrawRectangle(static_cast<int>(std::round(objXPx - objWidthPx / 2)),
                          static_cast<int>(std::round(objYPx - objHeightPx / 2)),
                          static_cast<int>(std::round(objWidthPx)),
                          static_cast<int>(std::round(objHeightPx)),
                          entity.hasColor() ? std::bit_cast<::Color>(entity.color()) : BLACK);
        }
    }
}
