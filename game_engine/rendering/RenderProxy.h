//
// Created by greg8 on 3/12/2026.
//

#ifndef GAME_ENGINE_RENDERPROXY_H
#define GAME_ENGINE_RENDERPROXY_H
#include "engine_core/EngineCore.h"

namespace Rendering
{
    using namespace EngineCore;

    struct  RenderProxy
    {
        float x, y;
        float width, height;

        bool hasColor;
        GameColor color;

        bool hasSprite;
        SpriteId spriteId;

        bool hasAnimationFrameIndex;
        uint16_t animationFrameIndex;

        bool hasType;
        EntityTypeId typeId;

        size_t index;

        uint8_t renderLayer;
    };
}

#endif //GAME_ENGINE_RENDERPROXY_H