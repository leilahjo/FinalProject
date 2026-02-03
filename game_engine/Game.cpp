//
// Created by greg8 on 1/29/2026.
//

#include "Game.h"

namespace Game
{
    // Define gravity to be 1 world unit per second squared in the downward Y direction.
    static constexpr float g = -1;

    Game::Game()
    {
        // Player
        size_t index = kinematicsData.create(0, 0, 0, 0);
        objects.push_back({&kinematicsData, index, 0.2, 0.2, 0, 1, GREEN, true});

        // Double-unit "border" squares
        index = kinematicsData.create(0, 0, 0, 0);
        objects.push_back({&kinematicsData, index, 2, 2, 0, 0, BLUE, true});
        index = kinematicsData.create(0, 0, 0, 0);
        objects.push_back({&kinematicsData, index, 1.9, 1.9, 0, 0, RAYWHITE, true});

        // Bouncing circles
        index = kinematicsData.create(-0.5, -0.5, 0.0, 0);
        objects.push_back({&kinematicsData, index, 0, 0, 0.1, 0.1, GOLD, false});
        index = kinematicsData.create(-0.25, -0.25, 0.0, 0);
        objects.push_back({&kinematicsData, index, 0, 0, 0.1, 0.1, MAROON, false});
        index = kinematicsData.create(0.25, 0.25, 0.0, 0);
        objects.push_back({&kinematicsData, index, 0, 0, 0.1, 0.1, PURPLE, false});
        index = kinematicsData.create(0.5, 0.5, 0.0, 0);
        objects.push_back({&kinematicsData, index, 0, 0, 0.1, 0.1, BEIGE, false});

        // Extremely dangerous. (why?)
        player = &objects[0];
    }

    void Game::Update(InputManager& inputManager)
    {
        // Input directly affects player velocity.
        player->vx() = 0;
        if (inputManager.moveLeft)
            player->vx() = -player->speed;
        if (inputManager.moveRight)
            player->vx() = player->speed;

        player->vy() = 0;
        if (inputManager.moveUp)
            player->vy() = player->speed;
        if (inputManager.moveDown)
            player->vy() = -player->speed;

        // Semi-Implicit Euler method
        player->x() += player->vx() * frameDt;
        player->y() += player->vy() * frameDt;
        for (int i = 0; i < objects.size(); ++i)
        {
            auto& object = objects[i];
            // Gravity only applies to our circles (for now).
            if (!object.isRect)
                object.vy() += g * frameDt;
        }
        for (int i = 0; i < kinematicsData.getCount(); i++)
        {
            kinematicsData.x[i] += kinematicsData.vx[i] * frameDt;
            kinematicsData.y[i] += kinematicsData.vy[i] * frameDt;
        }

        // Bounce logic
        for (int i = 0; i < objects.size(); ++i)
        {
            auto& object = objects[i];
            if (object.isRect)
                continue;

            if (object.y() < -1)
            {
                object.vy() *= -1;
                object.y() += 0.01f;
            }
        }
    }
}
