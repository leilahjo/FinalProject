//
// Created by greg8 on 1/29/2026.
//

#include "Game.h"

#include "Random.h"

namespace Game
{
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
        index = kinematicsData.create(-0.5, -0.5, randomFloat(-1, 1), randomFloat(-1, 1));
        objects.push_back({&kinematicsData, index, 0, 0, 0.1, 0.1, GOLD, false});
        index = kinematicsData.create(-0.25, -0.25,  randomFloat(-1, 1), randomFloat(-1, 1));
        objects.push_back({&kinematicsData, index, 0, 0, 0.1, 0.1, MAROON, false});
        index = kinematicsData.create(0.25, 0.25, randomFloat(-1, 1), randomFloat(-1, 1));
        objects.push_back({&kinematicsData, index, 0, 0, 0.1, 0.1, PURPLE, false});
        index = kinematicsData.create(0.5, 0.5, randomFloat(-1, 1), randomFloat(-1, 1));
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

            // Bottom
            if (object.y() < -1)
            {
                object.vy() *= -1;
                object.y() += 0.01f;
            }
            // Top
            if (object.y() > 1)
            {
                object.vy() *= -1;
                object.y() -= 0.01f;
            }
            // Left
            if (object.x() < -1)
            {
                object.vx() *= -1;
                object.x() += 0.01f;
            }
            // Right
            if (object.x() > 1)
            {
                object.vx() *= -1;
                object.x() -= 0.01f;
            }
        }
    }
}
