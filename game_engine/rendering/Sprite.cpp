//
// Created by greg8 on 2/21/2026.
//

#include "Sprite.h"

#include "raylib.h"

namespace Rendering
{
    Sprite::Sprite(const std::string& sourceFile, uint16_t colCt, uint16_t rowCt, uint16_t crop)
        : colCt(colCt),
          rowCt(rowCt),
          colWidth(0),
          rowHeight(0),
          crop(crop)
    {
        texture2D = LoadTexture(sourceFile.c_str());
        colWidth = static_cast<uint32_t>(texture2D.width) / colCt;
        rowHeight = static_cast<uint32_t>(texture2D.height) / rowCt;
    }

    Rectangle Sprite::GetSourceRect(uint16_t frameIndex)
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

    Rectangle Sprite::GetSourceRect()
    {
        return Rectangle{
            static_cast<float>(crop),
            static_cast<float>(crop),
            static_cast<float>(colWidth - crop * 2),
            static_cast<float>(rowHeight - crop * 2)
        };
    }
}
