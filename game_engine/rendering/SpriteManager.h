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

    struct SpriteManager
    {
        void initialize(uint16_t maxSprites);
        void uninitialize();

        SpriteId loadSprite(const std::string& sourceFile,
                              uint16_t colCt = 1, uint16_t rowCt = 1,
                              uint16_t crop = 0);
        bool unloadSprite(SpriteId id);
        Sprite* getSprite(SpriteId id);

    private:
        std::vector<Sprite> sprites;
        std::vector<uint16_t> freeIndices;
    };
}

#endif //RENDERER_SPRITEMANAGER_H
