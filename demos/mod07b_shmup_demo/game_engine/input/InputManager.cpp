//
// Created by greg8 on 1/27/2026.
//

#include "InputManager.h"

#include "raylib.h"

namespace Input
{
    void InputManager::update()
    {
        this->keyLeft = IsKeyDown(KEY_LEFT);
        this->keyRight = IsKeyDown(KEY_RIGHT);
        this->keyDown = IsKeyDown(KEY_DOWN);
        this->keyUp = IsKeyDown(KEY_UP);

        this->keyOne = IsKeyDown(KEY_ONE);
        this->keyTwo = IsKeyDown(KEY_TWO);
        this->keyThree = IsKeyDown(KEY_THREE);

        this->keySpace = IsKeyDown(KEY_SPACE);
    }
}
