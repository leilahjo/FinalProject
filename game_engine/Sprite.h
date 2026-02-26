//
// Created by greg8 on 11/19/2025.
//

#ifndef GAME_ENGINE_TEXTURE_H
#define GAME_ENGINE_TEXTURE_H

#include <cstdint>
#include <string>
#include "raylib.h"

namespace GameEngine
{
    struct Sprite
    {
        Texture2D texture2D;

        Sprite(const std::string& sourceFile, uint16_t colCt, uint16_t rowCt, uint16_t crop = 0);
        ~Sprite() = default;

        Rectangle GetSourceRect(uint16_t frameIndex);
        Rectangle GetSourceRect();

    private:
        uint16_t colCt;
        uint16_t rowCt;
        uint32_t colWidth;
        uint32_t rowHeight;
        uint16_t crop;
    };
}

#endif //GAME_ENGINE_TEXTURE_H
