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

        // Valid layers are 1-254
        std::unordered_map<EntityTypeId, uint8_t> typeToRenderLayer;
        RenderPreference typelessRenderPreference = RenderPreference::RENDER_FIRST;

        SwapBuffer swapBuffer;

        // Copy world / ECS data into a RenderFrame
        void frameSync(FrameData& frameData, World& world, RenderFrame& renderFrame);
        // Draw a RenderFrame
        void draw(uint64_t drawDurationUs, RenderFrame& renderFrame, BlockTimer& renderLoopTimer);

    private:
        void drawEntity(RenderProxy& renderProxy, RenderFrame& renderFrame);
        void sortRenderProxies(RenderFrame& renderFrame);
    };
}

#endif //GAME_ENGINE_RENDERER_H
