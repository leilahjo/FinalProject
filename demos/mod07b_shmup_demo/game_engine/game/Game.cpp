//
// Created by greg8 on 1/29/2026.
//

#include "Game.h"

#include <cmath>

namespace Game
{
    int Game::run()
    {
        return runtime.run(*this, 800, 1200, "GEP Shmup");
    }

    void Game::onStart(World& world)
    {
        particleArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_VELOCITY
            | Archetype::COMP_COLOR
            | Archetype::COMP_SIZE
            | Archetype::COMP_ANIMATION
            | Archetype::COMP_STATE);

        particleAlphaFadeAnimationId = world.animationSystem.createAnimation(STOP_AT_END, 1.0f);

        world.collisionSystem.enableCollisions(LAYER_PLAYER, LAYER_HOSTILE_SHIP_PROJECTILES, true);
        world.collisionSystem.enableCollisions(LAYER_PLAYER, LAYER_HOSTILE_SHIPS, true);
        world.collisionSystem.enableCollisions(LAYER_HOSTILE_SHIPS, LAYER_LASERS, true);

        starField.init(*this, world);
        playerShip.init(*this, world);
        hostileShipManager.init(*this, world);
    }

    void Game::onUpdateBegin(World& world)
    {
        playerShip.update(*this, world);
        hostileShipManager.update(*this, world);
        updateParticles();
    }

    void Game::onUpdatePostKinematics(World& world)
    {
        playerShip.enforceBoundary(*this, world);
        starField.enforceBoundary(*this, world);
        hostileShipManager.enforceBoundary(*this, world);
    }

    void Game::onUpdatePostCollisionDetection(World& world, std::vector<Collision>& collisions)
    {
        for (auto collision : collisions)
        {
            auto entityA = world.findEntity(collision.a).value();
            auto entityB = world.findEntity(collision.b).value();

            if (entityA.colliderLayerId() == LAYER_LASERS || entityA.colliderLayerId() == LAYER_HOSTILE_SHIPS)
            {
                entityA.state() |= Entity::STATE_DESTROYED;
                if (entityA.colliderLayerId() == LAYER_HOSTILE_SHIPS)
                    explode(entityA, world);
            }
            if (entityB.colliderLayerId() == LAYER_LASERS || entityB.colliderLayerId() == LAYER_HOSTILE_SHIPS)
            {
                entityB.state() |= Entity::STATE_DESTROYED;
                if (entityB.colliderLayerId() == LAYER_HOSTILE_SHIPS)
                    explode(entityB, world);
            }
        }

        // Prevent engine resolution of collisions.
        collisions.clear();
    }

    void Game::onUpdatePostCollisionResolution(World& world, std::vector<Collision>& collisions)
    {
    }

    void Game::explode(Entity entity, World& world)
    {
        //Spawn 100 particles
        for (int i = 0; i < 100; i++)
        {
            float x = entity.x();
            float y = entity.y();
            float width = 0.1f;
            float height = 0.1f;
            float speed = randomFloat(0.1, 0.5);
            float angle = randomFloat(0, 2 * PI);
            float vx = cos(angle) * speed;
            float vy = sin(angle) * speed;
            GameColor color = {
                static_cast<unsigned char>(255 * randomFloat(0, 1)),
                static_cast<unsigned char>(255 * randomFloat(0, 1)),
                static_cast<unsigned char>(255 * randomFloat(0, 1)),
                255
            };
            world.createEntity(particleArchetype,
                               x, y,
                               vx, vy,
                               width, height,
                               color,
                               Entity::STATE_DEFAULT,
                               ColliderShape::RECT, LAYER_NONE,
                               {particleAlphaFadeAnimationId, 0, 1.0f, AnimationData::State::PLAYING},
                               INVALID_SPRITE_ID
            );
        }
    }

    void Game::updateParticles()
    {
        // TODO do this with entity type
        for (EntityIndex entityIndex = 0; entityIndex < particleArchetype->getEntityCount(); entityIndex++)
        {
            auto entity = Entity{EntityLocation{particleArchetype, entityIndex}};
            auto& color = entity.color();
            color.a = 255 * (1 - entity.animation().progress);
            if (color.a == 0)
                entity.state() |= Entity::STATE_DESTROYED;
        }
    }
}
