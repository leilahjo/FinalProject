//
// Created by greg8 on 2/21/2026.
//

#include "Sprite.h"

#include "raylib.h"

namespace GameEngine
{
    Sprite::Sprite(const std::string& sourceFile, uint16_t colCt, uint16_t rowCt, uint16_t crop)
        : colCt(colCt),
          rowCt(rowCt),
          colWidth(0),
          rowHeight(0),
          crop(crop)
    {
        texture2D = LoadTexture(sourceFile.c_str());
        colWidth = texture2D.width / colCt;
        rowHeight = texture2D.height / rowCt;
    }

    Rectangle Sprite::GetSourceRect(uint16_t frameIndex)
    {
        uint16_t col = frameIndex % colCt;
        uint16_t row = frameIndex / colCt;

        return {
            static_cast<float>(col * colWidth),
            static_cast<float>(row * rowHeight),
            static_cast<float>(colWidth),
            static_cast<float>(rowHeight)
        };
    }

    Rectangle Sprite::GetSourceRect()
    {
        return {
            0,
            0,
            static_cast<float>(colWidth),
            static_cast<float>(rowHeight)
        };
    }
}
