//
// Created by greg8 on 1/29/2026.
//

#include "Game.h"

namespace Game
{
	// Define gravity to be 1 world unit per second squared in the downward Y direction.
    static constexpr float g = -1;

    void Game::Update(InputManager& inputManager)
    {
        // Input directly effects player velocity.
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

        // Semi-Implicit Euler method
        player.x += player.vx * frameDt;
        player.y += player.vy * frameDt;
        for (int i = 0; i < objects.size(); ++i)
        {
            auto& object = objects[i];
			// Gravity only applies to our circles (for now).
            if (!object.isRect)
                object.vy += g * frameDt;
            object.x += object.vx * frameDt;
            object.y += object.vy * frameDt;
        }

        // Bounce logic
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