//
// Created by greg8 on 11/19/2025.
//

#ifndef GAME_ENGINE_TEXTUREMANAGER_H
#define GAME_ENGINE_TEXTUREMANAGER_H
#include <cstdint>
#include <string>
#include <vector>

#include "ECSTypes.h"
#include "Sprite.h"

namespace GameEngine
{
    class SpriteManager
    {
        std::vector<Sprite> sprites;

    public:
        SpriteId createSprite(const std::string& sourceFile, uint16_t colCt, uint16_t rowCt, uint16_t crop = 0);
        Sprite* getSprite(SpriteId id);
    };
}

#endif //GAME_ENGINE_TEXTUREMANAGER_H
