//
// Created by greg8 on 1/29/2026.
//

#include "Game.h"

#include <cmath>

namespace Game
{
    int Game::run()
    {
        return runtime.run(*this, 1600, 1200, "GEP");
    }

    void Game::onStart(World& world)
    {
        world.collisionSystem.enableCollisions(LAYER_ANIMATED_SQUARES, LAYER_ROCKS, true);
        world.collisionSystem.enableCollisions(LAYER_ANIMATED_SQUARES, LAYER_ANIMATED_SQUARES, true);
        world.collisionSystem.enableCollisions(LAYER_ROCKS, LAYER_ROCKS, true);
        world.collisionSystem.enableCollisions(LAYER_PLAYER, LAYER_ROCKS, true);
        world.collisionSystem.enableCollisions(LAYER_PLAYER, LAYER_FAUNA, true);
        world.collisionSystem.enableCollisions(LAYER_FAUNA, LAYER_FAUNA, true);
        world.collisionSystem.enableCollisions(LAYER_FAUNA, LAYER_ROCKS, true);

        runtime.renderer.typeToRenderLayer[ENTITY_TYPE_ANIMATED_SQUARE] = 1;
        runtime.renderer.typeToRenderLayer[ENTITY_TYPE_FAUNA] = 2;
        runtime.renderer.typeToRenderLayer[ENTITY_TYPE_ROCK] = 3;
        runtime.renderer.typeToRenderLayer[ENTITY_TYPE_PLAYER] = 4;

        simpleAnimationId = world.animationSystem.createAnimation(STOP_AT_END, 1);
        playerRunAnimationId = world.animationSystem.createAnimation(LOOP, 1, 6 * 0, 6);
        animalIdleAnimation = world.animationSystem.createAnimation(LOOP, 1, 0, 4);

        rockSpriteId = runtime.renderer.spriteManager.loadSprite("assets/rock.png", 1, 1, 0);
        // runtime.renderer.spriteManager.unloadSprite(rockSpriteId);
        playerRunSpriteSheetId = runtime.renderer.spriteManager.loadSprite("assets/player_run.png", 6, 4, 5);
        lemmingSpriteId = runtime.renderer.spriteManager.loadSprite("assets/lemming.png");
        chickenSpriteSheetId = runtime.renderer.spriteManager.loadSprite("assets/chicken_idle.png", 4, 1);
        sheepSpriteSheetId = runtime.renderer.spriteManager.loadSprite("assets/sheep_idle.png", 4, 1);
        pigSpriteSheetId = runtime.renderer.spriteManager.loadSprite("assets/pig_idle.png", 4, 1);

        dootSoundId = runtime.audioManager.loadAudioAsset("assets/doot.wav", 10, Audio::REPLACE);
        // runtime.audioManager.unloadAudioAsset(dootSoundId);

        lemmingBehaviorId = runtime.behaviorManager.loadBehavior(
            (std::filesystem::path(PROJECT_ROOT) / "assets/lemming.lua").string()
        );
        faunaBehaviorId = runtime.behaviorManager.loadBehavior(
            (std::filesystem::path(PROJECT_ROOT) / "assets/fauna.lua").string()
        );

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
            | Archetype::COMP_SPRITE
            | Archetype::COMP_TYPE);
        rockArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_VELOCITY
            | Archetype::COMP_SIZE
            | Archetype::COMP_STATE
            | Archetype::COMP_COLLIDER
            | Archetype::COMP_SPRITE
            | Archetype::COMP_TYPE);
        animatedSquareArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_VELOCITY
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLOR
            | Archetype::COMP_STATE
            | Archetype::COMP_COLLIDER
            | Archetype::COMP_ANIMATION
			| Archetype::COMP_TYPE);
        faunaArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_VELOCITY
            | Archetype::COMP_SIZE
            | Archetype::COMP_STATE
            | Archetype::COMP_COLLIDER
            | Archetype::COMP_ANIMATION
            | Archetype::COMP_SPRITE
            | Archetype::COMP_BEHAVIOR
            | Archetype::COMP_TYPE);

        runtime.prefabManager.registerPrefab("sheep", Prefab{
                                                 faunaArchetype,
                                                 sheepSpriteSheetId,
                                                 0.10, 0.10,
                                                 white,
                                                 Entity::STATE_DEFAULT,
                                                 ColliderShape::RECT, LAYER_FAUNA,
                                                 AnimationData{animalIdleAnimation, -1, -1, AnimationData::PLAYING},
                                                 ENTITY_TYPE_FAUNA,
                                                 faunaBehaviorId
                                             });
        runtime.prefabManager.registerPrefab("chicken", Prefab{
                                                 faunaArchetype,
                                                 chickenSpriteSheetId,
                                                 0.10, 0.10,
                                                 white,
                                                 Entity::STATE_DEFAULT,
                                                 ColliderShape::RECT, LAYER_FAUNA,
                                                 AnimationData{animalIdleAnimation, -1, -1, AnimationData::PLAYING},
                                                 ENTITY_TYPE_FAUNA,
                                                 faunaBehaviorId
                                             });
        runtime.prefabManager.registerPrefab("pig", Prefab{
                                                 faunaArchetype,
                                                 pigSpriteSheetId,
                                                 0.10, 0.10,
                                                 white,
                                                 Entity::STATE_DEFAULT,
                                                 ColliderShape::RECT, LAYER_FAUNA,
                                                 AnimationData{animalIdleAnimation, -1, -1, AnimationData::PLAYING},
                                                 ENTITY_TYPE_FAUNA,
                                                 faunaBehaviorId
                                             });

        playerId = world.createEntity(playerArchetype,
                                      0, 0,
                                      0, 0,
                                      0.2, 0.2,
                                      green,
                                      Entity::STATE_DEFAULT,
                                      ColliderShape::RECT, LAYER_PLAYER,
                                      {},
                                      playerRunSpriteSheetId,
                                      ENTITY_TYPE_PLAYER,
                                      INVALID_BEHAVIOR_ID);
        AnimationSystem::startAnimation(world, playerId, playerRunAnimationId);
        runtime.behaviorManager.setGlobalEntityId("PLAYER_ID", playerId);

        // Double-unit "border" squares
        world.createEntity(borderArchetype,
                           0, 0,
                           0, 0,
                           4, 4,
                           yellow,
                           Entity::STATE_DEFAULT,
                           ColliderShape::RECT, LAYER_NONE,
                           {},
                           INVALID_SPRITE_ID,
                           ENTITY_TYPE_NONE,
                           INVALID_BEHAVIOR_ID);
        world.createEntity(borderArchetype,
                           0, 0,
                           0, 0,
                           3.95, 3.95,
                           rayWhite,
                           Entity::STATE_DEFAULT,
                           ColliderShape::RECT, LAYER_NONE,
                           {},
                           INVALID_SPRITE_ID,
                           ENTITY_TYPE_NONE,
                           INVALID_BEHAVIOR_ID);
        for (int i = 1; i < 10; i++)
            world.createEntity(rockArchetype,
                               0, 0,
                               randomFloat(-0.25, 0.25), randomFloat(-0.25, 0.25),
                               0.1 * 128 / 92, 0.1,
                               red,
                               Entity::STATE_DEFAULT,
                               ColliderShape::RECT, LAYER_ROCKS,
                               {},
                               rockSpriteId,
                               ENTITY_TYPE_ROCK,
                               INVALID_BEHAVIOR_ID);
        for (int i = 0; i < 10; i++)
            world.createEntity(faunaArchetype,
                               randomFloat(-1, 1), randomFloat(-1, 1),
                               0, 0,
                               0.1 * 64 / 144, 0.1,
                               red,
                               Entity::STATE_DEFAULT,
                               ColliderShape::RECT, LAYER_FAUNA,
                               {},
                               lemmingSpriteId,
                               ENTITY_TYPE_FAUNA,
                               lemmingBehaviorId
            );
        for (int i = 0; i < 50; i++)
            runtime.prefabManager.spawnPrefab("pig", world,
                                              randomFloat(-2, 2),
                                              randomFloat(-2, 2));
        for (int i = 0; i < 50; i++)
            runtime.prefabManager.spawnPrefab("sheep", world,
                                              randomFloat(-2, 2),
                                              randomFloat(-2, 2));
        for (int i = 0; i < 50; i++)
            runtime.prefabManager.spawnPrefab("chicken", world,
                                              randomFloat(-2, 2),
                                              randomFloat(-2, 2));
    }

    void Game::onUpdateBegin(World& world)
    {
        // Input directly affects player velocity.
        Entity player = world.findEntity(playerId).value();
        player.vx() = 0;
        if (runtime.inputManager.keyLeft)
            player.vx() = -1;
        if (runtime.inputManager.keyRight)
            player.vx() = 1;

        player.vy() = 0;
        if (runtime.inputManager.keyUp)
            player.vy() = 1;
        if (runtime.inputManager.keyDown)
            player.vy() = -1;


        if (runtime.inputManager.keySpace)
        {
            for (int i = 0; i < 1; i++)
            {
                world.createEntity(animatedSquareArchetype,
                                   player.x(), player.y(),
                                   randomFloat(-0.25, 0.25),
                                   randomFloat(-0.25, 0.25),
                                   0.025, 0.025,
                                   blue,
                                   Entity::STATE_DEFAULT,
                                   ColliderShape::RECT, LAYER_ANIMATED_SQUARES,
                                   {},
                                   INVALID_SPRITE_ID,
                                   ENTITY_TYPE_ANIMATED_SQUARE,
                                   INVALID_BEHAVIOR_ID);
            }
        }

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
                if (entity.y() - entity.height() / 2 < -2 && archetype->hasState())
                    entity.state() |= Entity::STATE_DESTROYED;
                //Top
                if (entity.y() + entity.height() / 2 > 2)
                {
                    entity.vy() *= -1;
                    entity.y() -= 0.01f;
                }
                //Left
                if (entity.x() - entity.width() / 2 < -2)
                {
                    entity.vx() *= -1;
                    entity.x() += 0.01f;
                }
                //Right
                if (entity.x() + entity.width() / 2 > 2)
                {
                    entity.vx() *= -1;
                    entity.x() -= 0.01f;
                }
            }
        }
    }

    void Game::onUpdatePostKinematics(World& world)
    {
    }

    void Game::onUpdatePostCollisionDetection(World& world, Collision collisions[], size_t collisionCount)
    {
        for (size_t i = 0; i < collisionCount; i++)
        {
            Collision collision = collisions[i];

            auto entityA = world.findEntity(collision.a).value();
            auto entityB = world.findEntity(collision.b).value();

            if (!entityA.hasEntityType() || !entityB.hasEntityType())
                continue;

            if (entityA.entityTypeId() == ENTITY_TYPE_ANIMATED_SQUARE && entityB.entityTypeId() == ENTITY_TYPE_ROCK)
                entityA.state() |= Entity::STATE_DESTROYED;
            if (entityA.entityTypeId() == ENTITY_TYPE_ROCK && entityB.entityTypeId() == ENTITY_TYPE_ANIMATED_SQUARE)
                entityB.state() |= Entity::STATE_DESTROYED;
            if (entityA.entityTypeId() == ENTITY_TYPE_ANIMATED_SQUARE && entityB.entityTypeId() == ENTITY_TYPE_ANIMATED_SQUARE)
            {
                AnimationSystem::startAnimation(world, entityA.id(), simpleAnimationId);
                AnimationSystem::startAnimation(world, entityB.id(), simpleAnimationId);

                runtime.audioManager.playOneshot(dootSoundId);
            }
        }
    }

    void Game::onUpdatePostCollisionResolution(World& world, Collision collisions[], size_t collisionCount)
    {
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

        auto player = world.findEntity(playerId).value();
        world.viewportX = player.x();
        world.viewportY = player.y();
    }
}
