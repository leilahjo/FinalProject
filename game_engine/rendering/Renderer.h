//
// Created by greg8 on 1/27/2026.
//

#ifndef GAME_ENGINE_RENDERER_H
#define GAME_ENGINE_RENDERER_H

#include <unordered_map>

#include "RenderFrame.h"
#include "RenderProxy.h"
#include "engine_core/EngineCore.h"
#include "SpriteManager.h"
#include "SwapBuffer.h"
#include "engine_core/util/BlockTimer.h"

namespace Rendering
{
    using namespace EngineCore;

    enum struct RenderPreference
    {
        RENDER_LAST,
        RENDER_FIRST
    };

    struct Renderer final
    {
        SpriteManager spriteManager;

        std::unordered_map<EntityTypeId, uint8_t> typeToRenderLayer;
        RenderPreference typelessRenderPreference = RenderPreference::RENDER_FIRST;

        SwapBuffer swapBuffer;

        // Toggled at runtime (F1). The engine runtime flips this and sets it on the RenderFrame.
        bool debugRenderEnabled = false;

        // Parallax layers — registered by the game during onStart.
        // These are copied into each RenderFrame so the draw thread can read them safely.
        void addParallaxLayer(GameColor color, float parallaxFactor,
                              float tileWidth = 20.0f, float tileHeight = 20.0f,
                              SpriteId spriteId = INVALID_SPRITE_ID);

        void frameSync(FrameData& frameData, World& world, RenderFrame& renderFrame);
        void draw(uint64_t drawDurationUs, RenderFrame& renderFrame, BlockTimer& renderLoopTimer);

    private:
        ParallaxLayer parallaxLayers[RenderFrame::MAX_PARALLAX_LAYERS];
        size_t                     parallaxLayerCount = 0;

        void drawParallax(RenderFrame& renderFrame);
        void drawEntity(RenderProxy& renderProxy, RenderFrame& renderFrame);
        void drawDebugOverlay(RenderProxy& proxy, RenderFrame& renderFrame);
        void drawHud(RenderFrame& renderFrame);
        void sortRenderProxies(RenderFrame& renderFrame);
    };
}

#endif //GAME_ENGINE_RENDERER_H
