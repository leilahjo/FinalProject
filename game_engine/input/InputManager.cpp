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

        this->keySpace = IsKeyDown(KEY_SPACE);

        this->keyF5Pressed = IsKeyPressed(KEY_R);
    }
}
