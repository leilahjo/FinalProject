//
// Created by greg8 on 2/17/2026.
//

#include <vector>

#include "CollisionSystem.h"
#include "../World.h"
#include "engine_core/memory_management/ArenaAllocator.h"

namespace EngineCore
{
    void CollisionSystem::enableCollisions(ColliderLayerId a, ColliderLayerId b, bool enabled)
    {
        permittedLayerCollisions[a][b] = enabled;
        permittedLayerCollisions[b][a] = enabled;
    }

    Collision* CollisionSystem::detect(World& world, size_t& collisionCount, ArenaAllocator& allocator)
    {
        collisionCount = 0;
        Collision* collisions = nullptr;

        // Array of layers, where every array is a vector of entities
        std::vector<Entity> colliderEntities[MAX_COLLISION_LAYER_COUNT];
        world.forEach(Archetype::COMP_POSITION | Archetype::COMP_SIZE | Archetype::COMP_COLLIDER,
                      [&colliderEntities](Entity entity)
                      {
                          colliderEntities[entity.colliderLayerId()].push_back(entity);
                      });

        for (ColliderLayerId layerAId = 0; layerAId < MAX_COLLISION_LAYER_COUNT; layerAId++)
        {
            for (ColliderLayerId layerBId = 0; layerBId < MAX_COLLISION_LAYER_COUNT; layerBId++)
            {
                // Skip <b,a> since we already checked <a,b>.
                if (layerBId > layerAId)
                    break;
                if (!permittedLayerCollisions[layerAId][layerBId])
                    continue;

                auto& layerA = colliderEntities[layerAId];
                auto& layerB = colliderEntities[layerBId];

                for (size_t a = 0; a < colliderEntities[layerAId].size(); a++)
                {
                    for (size_t b = 0; b < colliderEntities[layerBId].size(); b++)
                    {
                        if (layerAId == layerBId && b >= a)
                            break;

                        auto& entityA = layerA[a];
                        auto& entityB = layerB[b];

                        // Check whether AABBs overlap.
                        // Note that these checks shouldn't be expected to have great cache locality. That's unavoidable
                        // because we don't organize our entity data spatially.
                        if (entityA.right() < entityB.left() ||
                            entityA.left() > entityB.right() ||
                            entityA.top() < entityB.bottom() ||
                            entityA.bottom() > entityB.top())
                        {
                            continue;
                        }

                        auto newCollisionPtr = allocator.allocate<Collision>(1);
                        if (newCollisionPtr == nullptr)
                            return collisions;
                        *newCollisionPtr = Collision{entityA.id(), entityB.id()};;
                        collisionCount++;
                        if (collisions == nullptr)
                            collisions = newCollisionPtr;
                    }
                }
            }
        }

        return collisions;
    }

    void CollisionSystem::resolve(World& world, Collision collisions[], size_t collisionCount)
    {
        for (size_t i = 0; i < collisionCount; i++)
        {
            Collision collision = collisions[i];

            auto optA = world.findEntity(collision.a);
            auto optB = world.findEntity(collision.b);
            if (!optA || !optB)
                continue;

            auto& a = optA.value();
            auto& b = optB.value();
            if (a.state() & Entity::STATE_DESTROYED || b.state() & Entity::STATE_DESTROYED)
                continue;

            separate(a, b);
        }
    }

    static constexpr float separationEpsilon = 0.0001f;

    void CollisionSystem::separate(Entity& a, Entity& b)
    {
        bool aHasVelocity = a.entityLocation.archetype->hasVelocity();
        bool bHasVelocity = b.entityLocation.archetype->hasVelocity();

        if (!aHasVelocity && !bHasVelocity)
            return;

        float moveA = 0.5f + separationEpsilon;
        float moveB = 0.5f + separationEpsilon;
        if (!aHasVelocity)
        {
            moveA = 0;
            moveB = 1 + separationEpsilon;
        }
        if (!bHasVelocity)
        {
            moveA = 1 + separationEpsilon;
            moveB = 0;
        }

        float xOverlap = std::min(a.right(), b.right()) - std::max(a.left(), b.left());
        float yOverlap = std::min(a.top(), b.top()) - std::max(a.bottom(), b.bottom());

        bool resolveX = xOverlap < yOverlap;
        float& aPosition = resolveX ? a.x() : a.y();
        float& aVelocity = resolveX ? a.vx() : a.vy();
        float& bPosition = resolveX ? b.x() : b.y();
        float& bVelocity = resolveX ? b.vx() : b.vy();
        float& overlap = resolveX ? xOverlap : yOverlap;

        // A is left/below of B
        if (aPosition < bPosition)
        {
            aPosition -= overlap * moveA;
            bPosition += overlap * moveB;
            if (aVelocity > 0) aVelocity *= -1;
            if (bVelocity < 0) bVelocity *= -1;
        }
        else // A is right/top of B
        {
            aPosition += overlap * moveA;
            bPosition -= overlap * moveB;
            if (aVelocity < 0) aVelocity *= -1;
            if (bVelocity > 0) bVelocity *= -1;
        }
    }
}
