#include "Renderer.h"

#include <algorithm>
#include <cmath>
#include <bit>

#include "raylib.h"
#include "RenderProxy.h"
#include "engine_core/util/BlockTimer.h"

namespace Rendering
{
    void Renderer::addParallaxLayer(GameColor color, float parallaxFactor,
                                    float tileWidth, float tileHeight, SpriteId spriteId)
    {
        if (parallaxLayerCount >= RenderFrame::MAX_PARALLAX_LAYERS)
            return;
        auto& layer = parallaxLayers[parallaxLayerCount++];
        layer.color           = color;
        layer.parallaxFactor  = parallaxFactor;
        layer.tileWidth       = tileWidth;
        layer.tileHeight      = tileHeight;
        layer.hasSprite       = spriteId.index != static_cast<uint16_t>(-1);
        layer.spriteId        = spriteId;
    }

    void Renderer::frameSync(FrameData& frameData, World& world, RenderFrame& renderFrame)
    {
        renderFrame.backgroundColor  = world.backgroundColor;
        renderFrame.frameData        = frameData;
        renderFrame.viewportX        = world.viewportX;
        renderFrame.viewportY        = world.viewportY;
        renderFrame.viewportScale    = world.viewportScale;
        renderFrame.debugRenderEnabled = debugRenderEnabled;

        // Copy parallax layer specs so the render thread doesn't need the Renderer
        renderFrame.parallaxLayerCount = parallaxLayerCount;
        for (size_t i = 0; i < parallaxLayerCount; i++)
            renderFrame.parallaxLayers[i] = parallaxLayers[i];

        renderFrame.hud = world.hud;

        float sw = static_cast<float>(GetScreenWidth());
        float sh = static_cast<float>(GetScreenHeight());
        float minDim    = std::min(sw, sh);
        float pxPerUnit = minDim / world.viewportScale;

        float vLeft  = world.viewportX - sw / (2 * pxPerUnit);
        float vRight = world.viewportX + sw / (2 * pxPerUnit);
        float vTop   = world.viewportY + sh / (2 * pxPerUnit);
        float vBot   = world.viewportY - sh / (2 * pxPerUnit);

        renderFrame.renderProxies    = nullptr;
        renderFrame.renderProxyCount = 0;
        renderFrame.allocator.resetFully();

        world.forEach(Archetype::COMP_POSITION | Archetype::COMP_SIZE,
            [&](Entity entity)
            {
                if (entity.left()   > vRight || entity.right()  < vLeft ||
                    entity.bottom() > vTop   || entity.top()    < vBot)
                    return;

                auto* p = renderFrame.allocator.allocate<RenderProxy>(1);
                if (!p) return;
                if (!renderFrame.renderProxies)
                    renderFrame.renderProxies = p;
                renderFrame.renderProxyCount++;

                p->x = entity.x();
                p->y = entity.y();
                p->width  = entity.width();
                p->height = entity.height();

                p->hasColor = entity.hasColor();
                if (p->hasColor) p->color = entity.color();

                p->hasSprite = entity.hasSprite();
                if (p->hasSprite) p->spriteId = entity.spriteId();

                p->hasAnimationFrameIndex = false;
                if (entity.hasAnimation() && p->hasSprite)
                {
                    auto* anim = world.animationSystem.getAnimation(entity.animation().animationId);
                    if (anim)
                    {
                        p->hasAnimationFrameIndex = true;
                        p->animationFrameIndex = anim->getCurrentFrameIndex(entity.animation().progress);
                    }
                }

                p->hasType = entity.hasEntityType();
                if (p->hasType) p->typeId = entity.entityTypeId();

                p->hasRotation = entity.hasRotation();
                p->rotation    = entity.hasRotation() ? entity.rotation() : 0.0f;

                p->hasVelocity = entity.hasVelocity();
                if (p->hasVelocity)
                {
                    p->vx = entity.vx();
                    p->vy = entity.vy();
                }

                p->hasCollider    = entity.hasCollider();
                p->colliderShape  = entity.hasCollider() ? entity.colliderShape() : ColliderShape::RECT;

                p->hasHp      = entity.hasHp();
                p->currentHp  = entity.hasHp() ? entity.currentHp() : 0.0f;
                p->maxHp      = entity.hasHp() ? entity.maxHp()     : 1.0f;

                p->index       = renderFrame.renderProxyCount;
                p->renderLayer = 0;
            });
    }

    void Renderer::sortRenderProxies(RenderFrame& frame)
    {
        for (size_t i = 0; i < frame.renderProxyCount; i++)
        {
            auto& p = frame.renderProxies[i];
            auto  it = typeToRenderLayer.end();
            if (p.hasType) it = typeToRenderLayer.find(p.typeId);
            if (it != typeToRenderLayer.end())
                p.renderLayer = it->second;
            else
                p.renderLayer = (typelessRenderPreference == RenderPreference::RENDER_FIRST)
                                ? 0 : 255;
        }

        std::sort(frame.renderProxies, frame.renderProxies + frame.renderProxyCount,
            [](const RenderProxy& a, const RenderProxy& b)
            {
                if (a.renderLayer != b.renderLayer) return a.renderLayer < b.renderLayer;
                if (a.y != b.y) return a.y < b.y;
                return a.index < b.index;
            });
    }

    void Renderer::drawParallax(RenderFrame& frame)
    {
        float sw = static_cast<float>(GetScreenWidth());
        float sh = static_cast<float>(GetScreenHeight());
        float minDim    = std::min(sw, sh);
        float pxPerUnit = minDim / frame.viewportScale;
        float t = static_cast<float>(GetTime());

        for (size_t i = 0; i < frame.parallaxLayerCount; i++)
        {
            auto& layer = frame.parallaxLayers[i];
            ::Color rc  = std::bit_cast<::Color>(layer.color);

            float spacing = layer.tileWidth * pxPerUnit;
            if (spacing < 4.0f) spacing = 4.0f;

            // Offset: viewport contribution + time-based auto-scroll at the parallax rate
            float baseOx = -frame.viewportX * layer.parallaxFactor * pxPerUnit
                           + t * layer.parallaxFactor * 12.0f;
            float baseOy =  frame.viewportY * layer.parallaxFactor * pxPerUnit
                           + t * layer.parallaxFactor *  6.0f;

            float ox = std::fmod(baseOx, spacing);
            float oy = std::fmod(baseOy, spacing);
            if (ox < 0) ox += spacing;
            if (oy < 0) oy += spacing;

            float dotR = 1.5f + layer.parallaxFactor * 2.5f;

            for (float py = oy - spacing; py < sh + spacing; py += spacing)
                for (float px = ox - spacing; px < sw + spacing; px += spacing)
                    DrawCircle(static_cast<int>(px), static_cast<int>(py),
                               dotR, rc);
        }
    }

    void Renderer::drawDebugOverlay(RenderProxy& p, RenderFrame& frame)
    {
        float sw = static_cast<float>(GetScreenWidth());
        float sh = static_cast<float>(GetScreenHeight());
        float minDim    = std::min(sw, sh);
        float pxPerUnit = minDim / frame.viewportScale;
        float cx = sw * 0.5f + (p.x - frame.viewportX) * pxPerUnit;
        float cy = sh * 0.5f - (p.y - frame.viewportY) * pxPerUnit;
        float pw = p.width  * pxPerUnit;
        float ph = p.height * pxPerUnit;

        // 1. Bounding box
        DrawRectangleLinesEx({cx - pw * 0.5f, cy - ph * 0.5f, pw, ph}, 1, GREEN);

        // 2. Velocity arrow
        if (p.hasVelocity)
        {
            float len = std::sqrt(p.vx * p.vx + p.vy * p.vy);
            if (len > 0.01f)
            {
                float scale = 20.0f;
                DrawLine(static_cast<int>(cx), static_cast<int>(cy),
                         static_cast<int>(cx + p.vx / len * scale),
                         static_cast<int>(cy - p.vy / len * scale),
                         BLUE);
            }
        }

        // 3. HP bar (above entity, only for entities that have HP)
        if (p.hasHp && p.maxHp > 0)
        {
            float barW  = pw;
            float barH  = 4.0f;
            float barY  = cy - ph * 0.5f - barH - 2.0f;
            float pct   = std::max(0.0f, p.currentHp / p.maxHp);
            DrawRectangle(static_cast<int>(cx - barW * 0.5f), static_cast<int>(barY),
                          static_cast<int>(barW), static_cast<int>(barH), DARKGRAY);
            DrawRectangle(static_cast<int>(cx - barW * 0.5f), static_cast<int>(barY),
                          static_cast<int>(barW * pct), static_cast<int>(barH), RED);
        }
    }

    void Renderer::drawEntity(RenderProxy& p, RenderFrame& frame)
    {
        float sw = static_cast<float>(GetScreenWidth());
        float sh = static_cast<float>(GetScreenHeight());
        float minDim    = std::min(sw, sh);
        float pxPerUnit = minDim / frame.viewportScale;

        float cx = sw * 0.5f + (p.x - frame.viewportX) * pxPerUnit;
        float cy = sh * 0.5f - (p.y - frame.viewportY) * pxPerUnit;
        float pw = p.width  * pxPerUnit;
        float ph = p.height * pxPerUnit;

        float rot = p.hasRotation ? p.rotation : 0.0f;

        Sprite* sprite = p.hasSprite ? spriteManager.getSprite(p.spriteId) : nullptr;

        if (sprite)
        {
            Rectangle src = p.hasAnimationFrameIndex
                          ? sprite->getSourceRect(p.animationFrameIndex)
                          : sprite->getSourceRect();

            ::Color tint = p.hasColor ? std::bit_cast<::Color>(p.color) : WHITE;

            DrawTexturePro(sprite->texture2D,
                src,
                Rectangle{cx - pw * 0.5f, cy - ph * 0.5f, pw, ph},
                Vector2{pw * 0.5f, ph * 0.5f},
                rot,
                tint);
        }
        else
        {
            ::Color c = p.hasColor ? std::bit_cast<::Color>(p.color) : BLACK;
            if (rot == 0.0f)
                DrawRectangle(static_cast<int>(cx - pw * 0.5f),
                              static_cast<int>(cy - ph * 0.5f),
                              static_cast<int>(pw), static_cast<int>(ph), c);
            else
                DrawRectanglePro(Rectangle{cx, cy, pw, ph},
                                 Vector2{pw * 0.5f, ph * 0.5f},
                                 rot, c);
        }
    }

    void Renderer::drawHud(RenderFrame& frame)
    {
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();

        // HP bar — top-left, below the perf line
        constexpr float barW = 200.0f, barH = 16.0f;
        constexpr float barX = 10.0f,  barY = 32.0f;
        float pct = frame.hud.playerHpMax > 0.0f
                  ? frame.hud.playerHpCurrent / frame.hud.playerHpMax : 0.0f;
        if (pct < 0.0f) pct = 0.0f;
        if (pct > 1.0f) pct = 1.0f;

        ::Color hpColor = pct > 0.60f ? GREEN : (pct > 0.30f ? YELLOW : RED);

        DrawRectangle(static_cast<int>(barX), static_cast<int>(barY),
                      static_cast<int>(barW), static_cast<int>(barH), DARKGRAY);
        DrawRectangle(static_cast<int>(barX), static_cast<int>(barY),
                      static_cast<int>(barW * pct), static_cast<int>(barH), hpColor);
        DrawText(TextFormat("HP  %.0f / %.0f",
                            frame.hud.playerHpCurrent, frame.hud.playerHpMax),
                 static_cast<int>(barX + barW + 8), static_cast<int>(barY), 16, LIGHTGRAY);

        // Wave / kills / time — top-right
        DrawText(TextFormat("Wave  %d", frame.hud.wave),   sw - 150, 10, 20, WHITE);
        DrawText(TextFormat("Kills %d", frame.hud.kills),  sw - 150, 34, 20, WHITE);
        DrawText(TextFormat("%.0f s",   frame.hud.survivalTime), sw - 150, 58, 20, LIGHTGRAY);

        // Game-Over overlay
        if (frame.hud.isDead)
        {
            DrawRectangle(0, 0, sw, sh, {0, 0, 0, 160});

            const char* title = "GAME OVER";
            int titleSize = 60;
            int titleW = MeasureText(title, titleSize);
            DrawText(title, (sw - titleW) / 2, sh / 2 - 70, titleSize, RED);

            const char* stats = TextFormat("Survived %.0f s  |  Wave %d  |  Kills %d",
                                           frame.hud.survivalTime,
                                           frame.hud.wave,
                                           frame.hud.kills);
            int statsW = MeasureText(stats, 22);
            DrawText(stats, (sw - statsW) / 2, sh / 2 + 10, 22, WHITE);

            const char* hint = "Press R to Restart";
            int hintW = MeasureText(hint, 22);
            DrawText(hint, (sw - hintW) / 2, sh / 2 + 48, 22, LIGHTGRAY);
        }
    }

    void Renderer::draw(uint64_t drawUs, RenderFrame& frame, BlockTimer& timer)
    {
        timer.startBlock("sortRenderProxies");
        sortRenderProxies(frame);

        timer.startBlock("setup");
        BeginDrawing();
        ClearBackground(std::bit_cast<::Color>(frame.backgroundColor));

        timer.startBlock("parallax");
        drawParallax(frame);

        timer.startBlock("draw");
        for (size_t i = 0; i < frame.renderProxyCount; i++)
        {
            drawEntity(frame.renderProxies[i], frame);
            if (frame.debugRenderEnabled)
                drawDebugOverlay(frame.renderProxies[i], frame);
        }

        // Engine perf line
        DrawText(TextFormat("FPS %.0f  Work %lld us  Draw %lld us",
                            frame.frameData.fps,
                            frame.frameData.workDurationUs,
                            drawUs), 8, 8, 18, DARKGRAY);

        timer.startBlock("hud");
        drawHud(frame);

        timer.startBlock("EndDrawing");
        EndDrawing();
    }
}
