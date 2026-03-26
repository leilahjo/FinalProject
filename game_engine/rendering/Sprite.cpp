//
// Created by greg8 on 2/21/2026.
//

#include "Sprite.h"

#include "raylib.h"

namespace Rendering
{
    void Sprite::load(const std::string& sourceFile, uint16_t colCt, uint16_t rowCt, uint16_t crop)
    {
        this->colCt = colCt;
        this->rowCt = rowCt;
        this->crop = crop;

        texture2D = LoadTexture(sourceFile.c_str());
        colWidth = static_cast<uint32_t>(texture2D.width) / colCt;
        rowHeight = static_cast<uint32_t>(texture2D.height) / rowCt;
    }

    void Sprite::unload()
    {
        if (IsTextureValid(texture2D))
        {
            UnloadTexture(texture2D);
            texture2D = {};
            generation++;
        }
    }

    Rectangle Sprite::getSourceRect(uint16_t frameIndex)
    {
        const uint16_t col = static_cast<uint16_t>(frameIndex % colCt);
        const uint16_t row = static_cast<uint16_t>(frameIndex / colCt);

        return Rectangle{
            static_cast<float>(col * colWidth + crop),
            static_cast<float>(row * rowHeight + crop),
            static_cast<float>(colWidth - crop * 2),
            static_cast<float>(rowHeight - crop * 2)
        };
    }

    Rectangle Sprite::getSourceRect()
    {
        return Rectangle{
            static_cast<float>(crop),
            static_cast<float>(crop),
            static_cast<float>(colWidth - crop * 2),
            static_cast<float>(rowHeight - crop * 2)
        };
    }

    Sprite::Sprite(Sprite&& other) noexcept
         : texture2D(other.texture2D)
           , generation(other.generation)
           , colCt(other.colCt)
           , rowCt(other.rowCt)
           , colWidth(other.colWidth)
           , rowHeight(other.rowHeight)
           , crop(other.crop)
    {
        other.texture2D = {};
        other.colCt = 1;
        other.rowCt = 1;
        other.colWidth = 0;
        other.rowHeight = 0;
        other.crop = 0;
        other.generation = 0;
    }
}
