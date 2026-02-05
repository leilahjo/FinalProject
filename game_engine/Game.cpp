//
// Created by greg8 on 1/29/2026.
//

#include "Game.h"

#include "Random.h"
#include "KinematicsSystem.h"

namespace Game
{
    Game::Game()
    {
        borderArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLOR);
        squareArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_VELOCITY
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLOR);

        player = world.createEntity(squareArchetype, 0, 0, 0, 0, 0.2, 0.2, GREEN);

        // Double-unit "border" squares
        world.createEntity(borderArchetype, 0, 0, 0, 0, 2, 2, BLUE);
        world.createEntity(borderArchetype, 0, 0, 0, 0, 1.95, 1.95, RAYWHITE);

        // Moving red squares
        for (int i = 1; i < 10; i++)
            world.createEntity(squareArchetype, 0, 0, randomFloat(-0.25, 0.25), randomFloat(-0.1, 0.1), 0.1, 0.1, RED);
    }

    void Game::Update(InputManager& inputManager)
    {
        // Input directly affects player velocity.
        player.vx() = 0;
        if (inputManager.moveLeft)
            player.vx() = -0.5f;
        if (inputManager.moveRight)
            player.vx() = 0.5f;

        player.vy() = 0;
        if (inputManager.moveUp)
            player.vy() = 0.5f;
        if (inputManager.moveDown)
            player.vy() = -0.5f;

        // Semi-Implicit Euler method
        player.x() += player.vx() * frameDt;
        player.y() += player.vy() * frameDt;

        KinematicsSystem::Update(world, frameDt);

        // Bounce logic
        for (auto& archetype : world.archetypes)
        {
            if (!archetype->hasPosition() || !archetype->hasVelocity() || !archetype->hasSize())
                continue;

            for (ArchIndex archIndex = 0; archIndex < archetype->getCount(); archIndex++)
            {
                auto entity = Entity{archetype.get(), archIndex};
                if (entity.archetype == player.archetype && entity.archIndex == player.archIndex)
                    continue;

                //Bottom
                if (entity.y() - entity.height() / 2 < -1)
                {
                    entity.vy() *= -1;
                    entity.y() += 0.01f;
                }
                //Top
                if (entity.y() + entity.height() / 2 > 1)
                {
                    entity.vy() *= -1;
                    entity.y() -= 0.01f;
                }
                //Left
                if (entity.x() - entity.width() / 2 < -1)
                {
                    entity.vx() *= -1;
                    entity.x() += 0.01f;
                }
                //Right
                if (entity.x() + entity.width() / 2 > 1)
                {
                    entity.vx() *= -1;
                    entity.x() -= 0.01f;
                }
            }

            if (inputManager.addEntity)
            {
                for (int i = 1; i < 100; i++)
                {
                    world.createEntity(squareArchetype, player.x(), player.y(), randomFloat(-0.25, 0.25),
                                       randomFloat(-0.1, 0.1), 0.025,
                                       0.025, BLUE);
                }
            }
        }
    }
}
