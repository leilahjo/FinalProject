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

    // A parallax background layer drawn before all game entities.
    // Each layer shifts proportionally to the viewport at its own rate,
    // creating the illusion of depth.
    struct ParallaxLayer
    {
        GameColor color;           // tint / solid color if no sprite
        bool      hasSprite = false;
        SpriteId  spriteId;
        float     parallaxFactor;  // 0 = fully static, 1 = moves with viewport
        float     tileWidth;       // tile size in world units
        float     tileHeight;
    };

    struct RenderFrame
    {
        GameColor backgroundColor;
        FrameData frameData;
        float     viewportX, viewportY;
        float     viewportScale = 2.0f; // pxPerUnit = minScreenDim / viewportScale

        // Debug overlay toggle — flipped by a key press in the runtime
        bool debugRenderEnabled = false;

        // Parallax layers registered by the game in onStart
        static constexpr size_t MAX_PARALLAX_LAYERS = 8;
        ParallaxLayer parallaxLayers[MAX_PARALLAX_LAYERS];
        size_t        parallaxLayerCount = 0;

        HudData hud;

        ArenaAllocator allocator = ArenaAllocator(100 * 1024 * 1024);
        RenderProxy*   renderProxies    = nullptr;
        uint32_t       renderProxyCount = 0;
    };
}

#endif //GAME_ENGINE_RENDERFRAME_H
