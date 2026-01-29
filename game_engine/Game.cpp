//
// Created by greg8 on 1/29/2026.
//

#include "Game.h"

namespace Game
{
    static constexpr float g = -1;

    void Game::Update(InputManager& inputManager)
    {
        player.vx = 0;
        if (inputManager.moveLeft)
            player.vx = -player.speed;
        if (inputManager.moveRight)
            player.vx = player.speed;

        player.vy = 0;
        if (inputManager.moveUp)
            player.vy = player.speed;
        if (inputManager.moveDown)
            player.vy = -player.speed;

        player.x += player.vx * frameDt;
        player.y += player.vy * frameDt;

        for (int i = 0; i < objects.size(); ++i)
        {
            auto& object = objects[i];
            if (!object.isRect)
                object.vy += g * frameDt;
            object.x += object.vx * frameDt;
            object.y += object.vy * frameDt;
        }

        for (int i = 0; i < objects.size(); ++i)
        {
            auto& object = objects[i];
            if (object.isRect)
                continue;

            if (object.y < -1)
            {
                object.vy *= -1;
                object.y += 0.01f;
            }
        }
    }
}