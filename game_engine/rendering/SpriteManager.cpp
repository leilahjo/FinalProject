//
// Created by greg8 on 11/19/2025.
//


#include "SpriteManager.h"

namespace Rendering
{
    void SpriteManager::initialize(uint16_t maxSprites)
    {
        sprites.resize(maxSprites);
        freeIndices.resize(maxSprites);
        for (size_t i = 0; i < freeIndices.size(); i++)
            freeIndices[i] = maxSprites - i - 1;
    }

    void SpriteManager::uninitialize()
    {
        for (auto& sprite : sprites)
            sprite.unload();
        sprites.clear();
        freeIndices.clear();
    }

    SpriteId SpriteManager::loadSprite(const std::string& sourceFile,
                                       uint16_t colCt, uint16_t rowCt,
                                       uint16_t crop)
    {
        if (freeIndices.empty())
            return INVALID_SPRITE_ID;

        uint16_t index = freeIndices.back();
        freeIndices.pop_back();
        sprites[index].load(sourceFile, colCt, rowCt, crop);
        return SpriteId{index, sprites[index].generation};
    }

    bool SpriteManager::unloadSprite(SpriteId id)
    {
        if (id.index >= sprites.size())
            return false;
        auto& sprite = sprites[id.index];
        if (sprite.generation != id.generation)
            return false;

        sprite.unload();
        freeIndices.push_back(id.index);
        return true;
    }

    Sprite* SpriteManager::getSprite(SpriteId id)
    {
        if (id.index >= sprites.size())
            return nullptr;
        auto sprite = &sprites[id.index];
        if (sprite->generation != id.generation)
            return nullptr;
        return &sprites[id.index];
    }
}
