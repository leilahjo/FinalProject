#include "EntityFactory.h"

#include "engine_core/util/Random.h"
#include <cmath>

namespace Game
{
    EntityId EntityFactory::spawnPlayer(World& world, float x, float y)
    {
        return world.createEntity(playerArch, EntitySpec{
            .x = x, .y = y,
            .width = 0.14f, .height = 0.14f,
            .color = white,
            .state = STATE_DEFAULT,
            .colliderShape = ColliderShape::CIRCLE, .colliderLayerId = layerPlayer,
            .animationData = {playerRunAnim, 0.0f, 1.0f, AnimationData::PLAYING},
            .spriteId = playerSprite,
            .entityType = typePlayer,
            .maxHp = 100.0f, .currentHp = 100.0f,
            .rotation = 0.0f,
        });
    }

    EntityId EntityFactory::spawnChaser(World& world, float x, float y)
    {
        return world.createEntity(chaserArch, EntitySpec{
            .x = x, .y = y,
            .width = 0.12f, .height = 0.12f,
            .color = {230, 50, 50, 255},
            .state = STATE_DEFAULT,
            .colliderShape = ColliderShape::CIRCLE, .colliderLayerId = layerEnemy,
            .entityType = typeChaser,
            .behaviorId = chaserBehaviorId,
            .maxHp = 30.0f, .currentHp = 30.0f,
        });
    }

    EntityId EntityFactory::spawnTank(World& world, float x, float y)
    {
        return world.createEntity(tankArch, EntitySpec{
            .x = x, .y = y,
            .width = 0.20f, .height = 0.20f,
            .color = {140, 40, 180, 255},
            .state = STATE_DEFAULT,
            .colliderShape = ColliderShape::CIRCLE, .colliderLayerId = layerEnemy,
            .entityType = typeTank,
            .maxHp = 90.0f, .currentHp = 90.0f,
        });
    }

    EntityId EntityFactory::spawnProj(World& world, float x, float y, float dx, float dy)
    {
        float angle = std::atan2(-dy, dx) * (180.0f / 3.14159265f);
        return world.createEntity(projArch, EntitySpec{
            .x = x, .y = y,
            .vx = dx * 5.0f, .vy = dy * 5.0f,
            .width = 0.07f, .height = 0.03f,
            .color = {255, 230, 80, 255},
            .state = STATE_DEFAULT,
            .colliderShape = ColliderShape::RECT, .colliderLayerId = layerProj,
            .entityType = typeProjectile,
            .rotation = angle,
            .particleLifetime = 1.6f, .particleMaxLifetime = 1.6f,
        });
    }

    EntityId EntityFactory::spawnPickup(World& world, float x, float y)
    {
        return world.createEntity(pickupArch, EntitySpec{
            .x = x, .y = y,
            .width = 0.10f, .height = 0.10f,
            .color = {50, 220, 80, 255},
            .state = STATE_DEFAULT,
            .colliderShape = ColliderShape::CIRCLE, .colliderLayerId = layerPickup,
            .entityType = typePickup,
            .particleLifetime = 8.0f, .particleMaxLifetime = 8.0f,
        });
    }

    void EntityFactory::spawnBurst(World& world, float x, float y, GameColor color, int n)
    {
        for (int i = 0; i < n; i++)
        {
            float angle = randomFloat(0.0f, 6.2832f);
            float speed = randomFloat(0.5f, 2.0f);
            float life  = randomFloat(0.3f, 0.7f);

            world.createEntity(particleArch, EntitySpec{
                .x = x, .y = y,
                .vx = std::cos(angle) * speed,
                .vy = std::sin(angle) * speed,
                .width  = randomFloat(0.02f, 0.06f),
                .height = randomFloat(0.02f, 0.06f),
                .color  = color,
                .state  = STATE_DEFAULT,
                .entityType = typeParticle,
                .particleLifetime    = life,
                .particleMaxLifetime = life,
            });
        }
    }
}
