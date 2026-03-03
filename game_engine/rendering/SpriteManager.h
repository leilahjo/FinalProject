//
// Created by greg8 on 11/19/2025.
//

#ifndef RENDERER_SPRITEMANAGER_H
#define RENDERER_SPRITEMANAGER_H
#include <cstdint>
#include <string>
#include <vector>

#include "Sprite.h"
#include "engine_core/EngineCore.h"

namespace Rendering
{
    using namespace EngineCore;

    class SpriteManager
    {
        std::vector<Sprite> sprites;

    public:
        SpriteId createSprite(const std::string& sourceFile,
                              uint16_t colCt = 1, uint16_t rowCt = 1,
                              uint16_t crop = 0);
        Sprite* getSprite(SpriteId id);
    };
}

#endif //RENDERER_SPRITEMANAGER_H
