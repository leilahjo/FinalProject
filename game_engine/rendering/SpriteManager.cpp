//
// Created by greg8 on 11/19/2025.
//


#include "SpriteManager.h"

namespace Rendering
{
    SpriteId SpriteManager::createSprite(const std::string& sourceFile,
                                         uint16_t colCt, uint16_t rowCt,
                                         uint16_t crop)
    {
        sprites.push_back(Sprite{sourceFile, colCt, rowCt, crop});
        return sprites.size() - 1;
    }

    Sprite* SpriteManager::getSprite(SpriteId id)
    {
        return &sprites[id];
    }
}
