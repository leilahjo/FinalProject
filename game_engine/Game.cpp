//
// Created by greg8 on 1/29/2026.
//

#include "Game.h"

#include <cmath>

#include "AnimationSystem.h"
#include "Entity.h"
#include "InputManager.h"
#include "Random.h"
#include "KinematicsSystem.h"

namespace Game
{
    Game::Game()
    {
        collisionSystem.enableCollisions(LAYER_BLUE_SQUARES, LAYER_RED_SQUARES, true);
        collisionSystem.enableCollisions(LAYER_BLUE_SQUARES, LAYER_BLUE_SQUARES, true);
        collisionSystem.enableCollisions(LAYER_RED_SQUARES, LAYER_RED_SQUARES, true);
        collisionSystem.enableCollisions(LAYER_PLAYER, LAYER_RED_SQUARES, true);

        simpleAnimationId = animationSystem.createAnimation(STOP_AT_END, 1);
        playerAnimationId = animationSystem.createAnimation(LOOP, 1, 6 * 0, 6);

        rockSpriteId = spriteManager.createSprite("assets/rock.png", 1, 1, 0);
        playerSpriteSheetId = spriteManager.createSprite("assets/player_run.png", 6, 4, 0);

        // Instead of permanently setting velocity to 0, we can avoid storing velocity altogether.
        borderArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLOR);
        playerArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_VELOCITY
            | Archetype::COMP_SIZE
            | Archetype::COMP_STATE
            | Archetype::COMP_COLLIDER
            | Archetype::COMP_ANIMATION
            | Archetype::COMP_SPRITE);
        rockArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_VELOCITY
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLOR
            | Archetype::COMP_STATE
            | Archetype::COMP_COLLIDER
            | Archetype::COMP_SPRITE);
        animatedSquareArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_VELOCITY
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLOR
            | Archetype::COMP_STATE
            | Archetype::COMP_COLLIDER
            | Archetype::COMP_ANIMATION);

        playerId = world.createEntity(playerArchetype,
                                      0, 0,
                                      0, 0,
                                      0.2, 0.2,
                                      GREEN,
                                      Entity::STATE_DEFAULT,
                                      ColliderShape::RECT, LAYER_PLAYER,
                                      {},
                                      playerSpriteSheetId);

        AnimationSystem::startAnimation(world, playerId, playerAnimationId);

        // Double-unit "border" squares
        world.createEntity(borderArchetype,
                           0, 0,
                           0, 0,
                           2, 2,
                           YELLOW,
                           Entity::STATE_DEFAULT,
                           ColliderShape::RECT, LAYER_NONE,
                           {},
                           INVALID_SPRITE_ID);
        world.createEntity(borderArchetype,
                           0, 0,
                           0, 0,
                           1.95, 1.95,
                           RAYWHITE,
                           Entity::STATE_DEFAULT,
                           ColliderShape::RECT, LAYER_NONE,
                           {},
                           INVALID_ANIMATION_ID);

        // Moving red squares
        for (int i = 1; i < 10; i++)
            world.createEntity(rockArchetype,
                               0, 0,
                               randomFloat(-0.25, 0.25), randomFloat(-0.25, 0.25),
                               0.1, 0.1 * 92 / 128,
                               WHITE,
                               Entity::STATE_DEFAULT,
                               ColliderShape::RECT, LAYER_RED_SQUARES,
                               {},
                               rockSpriteId);
    }

    void Game::Update(InputManager& inputManager)
    {
        // Input directly affects player velocity.
        Entity player = world.findEntity(playerId).value();
        player.vx() = 0;
        if (inputManager.moveLeft)
            player.vx() = -1;
        if (inputManager.moveRight)
            player.vx() = 1;

        player.vy() = 0;
        if (inputManager.moveUp)
            player.vy() = 1;
        if (inputManager.moveDown)
            player.vy() = -1;

        KinematicsSystem::Update(world, frameDt);

        // Bounce logic
        for (auto& archetype : world.archetypes)
        {
            if (!archetype->hasPosition() || !archetype->hasVelocity() || !archetype->hasSize())
                continue;

            for (EntityIndex entityIndex = 0; entityIndex < archetype->getEntityCount(); entityIndex++)
            {
                auto entity = Entity{archetype.get(), entityIndex};
                if (entity == player)
                    continue;

                //Bottom
                if (entity.y() - entity.height() / 2 < -1)
                    entity.state() |= Entity::STATE_DESTROYED;
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
        }

        if (inputManager.addEntity)
        {
            for (int i = 1; i < 10; i++)
            {
                world.createEntity(animatedSquareArchetype,
                                   player.x(), player.y(),
                                   randomFloat(-0.25, 0.25),
                                   randomFloat(-0.25, 0.25),
                                   0.025, 0.025,
                                   BLUE,
                                   Entity::STATE_DEFAULT,
                                   ColliderShape::RECT, LAYER_BLUE_SQUARES,
                                   {},
                                   INVALID_SPRITE_ID);
            }
        }

        auto collisions = collisionSystem.detect(world);
        for (auto& collision : collisions)
        {
            auto entityA = world.findEntity(collision.a).value();
            auto entityB = world.findEntity(collision.b).value();

            // This is a bit of a hack, because we don't have a better way to identify types of entities (yet).
            if (entityA.colliderLayerId() == LAYER_BLUE_SQUARES && entityB.colliderLayerId() == LAYER_RED_SQUARES)
                entityA.state() |= Entity::STATE_DESTROYED;
            if (entityA.colliderLayerId() == LAYER_RED_SQUARES && entityB.colliderLayerId() == LAYER_BLUE_SQUARES)
                entityB.state() |= Entity::STATE_DESTROYED;
            if (entityA.colliderLayerId() == LAYER_BLUE_SQUARES && entityB.colliderLayerId() == LAYER_BLUE_SQUARES)
            {
                AnimationSystem::startAnimation(world, entityA.id(), simpleAnimationId);
                AnimationSystem::startAnimation(world, entityB.id(), simpleAnimationId);
            }
        }
        collisionSystem.resolve(world, collisions);

        world.forEach(Archetype::COMP_ANIMATION | Archetype::COMP_COLOR,
                      [](Entity entity)
                      {
                          if (entity.animation().state == AnimationData::PLAYING || entity.color().r > 0)
                          {
                              entity.color().r = static_cast<unsigned char>(
                                  std::round(255.0f * (1.0f - entity.animation().progress))
                              );
                          }
                      });
        animationSystem.update(world, frameDt);

        world.cleanup();
    }
}
