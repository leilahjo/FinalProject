//
// Created by greg8 on 3/12/2026.
//

#ifndef GAME_ENGINE_RENDERFRAME_H
#define GAME_ENGINE_RENDERFRAME_H

#include "RenderProxy.h"
#include "engine_core/EngineCore.h"

namespace Rendering
{
    using namespace EngineCore;

    struct RenderFrame
    {
        GameColor backgroundColor;
        FrameData frameData;
        float viewportX, viewportY;

        ArenaAllocator allocator = ArenaAllocator(10 * 1024 * 1024);
        RenderProxy* renderProxies;
        uint32_t renderProxyCount;
    };
}

#endif //GAME_ENGINE_RENDERFRAME_H