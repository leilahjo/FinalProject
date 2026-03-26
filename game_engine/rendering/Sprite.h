//
// Created by greg8 on 11/19/2025.
//

#ifndef GAME_ENGINE_TEXTURE_H
#define GAME_ENGINE_TEXTURE_H

#include <cstdint>
#include <string>
#include "raylib.h"

namespace Rendering
{
    struct Sprite
    {
        Sprite() = default;

        void load(const std::string& sourceFile, uint16_t colCt = 1, uint16_t rowCt = 1, uint16_t crop = 0);
        void unload();

        Rectangle getSourceRect(uint16_t frameIndex);
        Rectangle getSourceRect();

        // Copy semantics.
        Sprite(const Sprite&) = delete;
        Sprite& operator=(const Sprite&) = delete;

        // Move semantics.
        Sprite& operator=(Sprite&& other) = delete;
        Sprite(Sprite&& other) noexcept;

        friend struct SpriteManager;
        friend struct Renderer;

    private:
        Texture2D texture2D{};
        uint32_t generation = 1;

        uint16_t colCt = 1;
        uint16_t rowCt = 1;
        uint32_t colWidth = 0;
        uint32_t rowHeight = 0;
        uint16_t crop = 0;
    };
}

#endif //GAME_ENGINE_TEXTURE_H
