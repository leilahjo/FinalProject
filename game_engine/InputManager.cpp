//
// Created by greg8 on 1/27/2026.
//

#include "InputManager.h"

#include "raylib.h"

void InputManager::Update()
{
    this->moveLeft = IsKeyDown(KEY_LEFT);
    this->moveRight = IsKeyDown(KEY_RIGHT);
    this->moveDown = IsKeyDown(KEY_DOWN);
    this->moveUp = IsKeyDown(KEY_UP);
}
