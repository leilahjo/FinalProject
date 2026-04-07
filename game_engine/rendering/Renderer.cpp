//
// Created by greg8 on 1/27/2026.
//

#include <cmath>

#include "Renderer.h"

#include <algorithm>

#include "raylib.h"
#include "RenderProxy.h"

namespace Rendering
{
    void Renderer::frameSync(FrameData& frameData, World& world)
    {
        renderFrame.backgroundColor = world.backgroundColor;
        renderFrame.frameData = frameData;
        renderFrame.viewportX = world.viewportX;
        renderFrame.viewportY = world.viewportY;

        float windowWidthPx = GetScreenWidth();
        float windowHeightPx = GetScreenHeight();
        float minDim = std::min(windowWidthPx, windowHeightPx);
        float pxPerGameUnit = minDim / 2;
        float viewPortWidth = windowWidthPx / pxPerGameUnit;
        float viewPortHeight = windowHeightPx / pxPerGameUnit;

        float viewPortLeft = world.viewportX - viewPortWidth / 2;
        float viewPortRight = world.viewportX + viewPortWidth / 2;
        float viewPortTop = world.viewportY + viewPortHeight / 2;
        float viewPortBottom = world.viewportY - viewPortHeight / 2;

        renderFrame.renderProxies = nullptr;
        renderFrame.renderProxyCount = 0;
        renderFrame.allocator.resetFully();
        world.forEach(Archetype::COMP_POSITION | Archetype::COMP_SIZE,
                      [&](Entity entity)
                      {
                          // View port culling.
                          if (entity.left() > viewPortRight
                              || entity.right() < viewPortLeft
                              || entity.bottom() > viewPortTop
                              || entity.top() < viewPortBottom)
                              return;

                          auto renderProxy = renderFrame.allocator.allocate<RenderProxy>(1);
                          if (renderProxy == nullptr)
                              return;
                          if (renderFrame.renderProxies == nullptr)
                              renderFrame.renderProxies = renderProxy;
                          renderFrame.renderProxyCount++;

                          renderProxy->hasType = entity.hasEntityType();
                          if (renderProxy->hasType)
                              renderProxy->typeId = entity.entityTypeId();

                          renderProxy->x = entity.x();
                          renderProxy->y = entity.y();

                          renderProxy->width = entity.width();
                          renderProxy->height = entity.height();

                          renderProxy->hasColor = entity.hasColor();
                          if (renderProxy->hasColor)
                              renderProxy->color = entity.color();

                          renderProxy->hasSprite = entity.hasSprite();
                          if (renderProxy->hasSprite)
                              renderProxy->spriteId = entity.spriteId();

                          if (entity.hasAnimation() && renderProxy->hasSprite)
                          {
                              Animation* animation = world.animationSystem.getAnimation(entity.animation().animationId);
                              if (animation)
                              {
                                  renderProxy->hasAnimationFrameIndex = true;
                                  renderProxy->animationFrameIndex = animation->getCurrentFrameIndex(
                                      entity.animation().progress);
                              }
                              else
                                  renderProxy->hasAnimationFrameIndex = false;
                          }
                          else
                              renderProxy->hasAnimationFrameIndex = false;

                          renderProxy->index = renderFrame.renderProxyCount;
                      });
    }

    void Renderer::sortRenderProxies()
    {
        for (size_t i = 0; i < renderFrame.renderProxyCount; i++)
        {
            auto& renderProxy = renderFrame.renderProxies[i];
            auto candidateLayer = typeToRenderLayer.end();
            if (renderProxy.hasType)
                candidateLayer = typeToRenderLayer.find(renderProxy.typeId);
            if (candidateLayer != typeToRenderLayer.end())
                renderProxy.renderLayer = candidateLayer->second;
            else
                renderProxy.renderLayer = typelessRenderPreference == RenderPreference::RENDER_FIRST
                                              ? 0
                                              : std::numeric_limits<uint8_t>::max();
        }

        std::sort(renderFrame.renderProxies,
            renderFrame.renderProxies + renderFrame.renderProxyCount,
                  [](const RenderProxy& a, const RenderProxy& b)
                  {
                      if (a.renderLayer != b.renderLayer)
                          return a.renderLayer < b.renderLayer;
                      if (a.y != b.y)
                          return a.y < b.y;
                      return a.index < b.index;
                  });
    }

    void Renderer::draw(uint64_t drawDurationUs)
    {
        sortRenderProxies();

        BeginDrawing();
        ClearBackground(std::bit_cast<::Color>(renderFrame.backgroundColor));

        for (size_t i = 0; i < renderFrame.renderProxyCount; i++)
        {
            auto& renderProxy = renderFrame.renderProxies[i];
            drawEntity(renderProxy);
        }

        DrawText(TextFormat("FPS %f\nJitter: %lld us\nWork: %lld us\nDraw: %lld us", renderFrame.frameData.fps,
                            renderFrame.frameData.jitterUs,
                            renderFrame.frameData.workDurationUs, drawDurationUs), 0, 0, 36, ::GRAY);

        EndDrawing();
    }

    void Renderer::drawEntity(RenderProxy& entity)
    {
        double windowWidthPx = GetScreenWidth();
        double windowHeightPx = GetScreenHeight();

        double minDim = std::min(windowWidthPx, windowHeightPx);
        double pxPerGameUnit = minDim / 2;

        double originXPx = windowWidthPx / 2;
        double originYPx = windowHeightPx / 2;

        double objXPx = originXPx + (entity.x - renderFrame.viewportX) * pxPerGameUnit;
        double objYPx = originYPx - (entity.y - renderFrame.viewportY) * pxPerGameUnit;

        double objWidthPx = entity.width * pxPerGameUnit;
        double objHeightPx = entity.height * pxPerGameUnit;

        Sprite* sprite = entity.hasSprite
                             ? spriteManager.getSprite(entity.spriteId)
                             : nullptr;

        if (sprite)
        {
            Rectangle srcRect;
            if (entity.hasAnimationFrameIndex)
                srcRect = sprite->getSourceRect(entity.animationFrameIndex);
            else
                srcRect = sprite->getSourceRect();
            DrawTexturePro(sprite->texture2D, srcRect, Rectangle{
                               static_cast<float>(objXPx - objWidthPx / 2),
                               static_cast<float>(objYPx - objHeightPx / 2),
                               static_cast<float>(objWidthPx),
                               static_cast<float>(objHeightPx)
                           },
                           {},
                           0,
                           entity.hasColor ? std::bit_cast<::Color>(entity.color) : WHITE);
        }
        else
        {
            DrawRectangle(static_cast<int>(std::round(objXPx - objWidthPx / 2)),
                          static_cast<int>(std::round(objYPx - objHeightPx / 2)),
                          static_cast<int>(std::round(objWidthPx)),
                          static_cast<int>(std::round(objHeightPx)),
                          entity.hasColor ? std::bit_cast<::Color>(entity.color) : BLACK);
        }
    }
}
