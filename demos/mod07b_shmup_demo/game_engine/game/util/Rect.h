//
// Created by greg8 on 2/15/2026.
//

#ifndef GAME_ENGINE_RECT_H
#define GAME_ENGINE_RECT_H

namespace Game
{
    struct Rect
    {
        float left, right, bottom, top;

        float width();

        float height();
    };
}


#endif //GAME_ENGINE_RECT_H
