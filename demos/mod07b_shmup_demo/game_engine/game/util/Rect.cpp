//
// Created by greg8 on 2/15/2026.
//

#include "Rect.h"

float Game::Rect::width()
{
    return right - left;
}

float Game::Rect::height()
{
    return bottom - top;
}
