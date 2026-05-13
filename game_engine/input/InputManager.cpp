//
// Created by greg8 on 1/27/2026.
//

#include "InputManager.h"

#include <algorithm>
#include "raylib.h"

namespace Input
{
    void InputManager::update(float viewportX, float viewportY, float viewportScale)
    {
        keyLeft  = IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A);
        keyRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
        keyDown  = IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S);
        keyUp    = IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W);
        keySpace = IsKeyDown(KEY_SPACE);

        keyF5Pressed = IsKeyPressed(KEY_F5); // Lua hot-reload
        keyF1Pressed = IsKeyPressed(KEY_F1);
        keyRPressed  = IsKeyPressed(KEY_R);

        mouseLeft        = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        mouseLeftPressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        Vector2 pos = GetMousePosition();
        mouseScreenX = pos.x;
        mouseScreenY = pos.y;

        // Convert screen pixels → world units using the same formula as the renderer.
        float windowW = static_cast<float>(GetScreenWidth());
        float windowH = static_cast<float>(GetScreenHeight());
        float minDim  = std::min(windowW, windowH);
        float pxPerUnit = minDim / viewportScale;

        mouseWorldX = viewportX + (mouseScreenX - windowW * 0.5f) / pxPerUnit;
        mouseWorldY = viewportY - (mouseScreenY - windowH * 0.5f) / pxPerUnit;
    }
}
