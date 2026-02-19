//
// Created by greg8 on 2/17/2026.
//

#include "CollisionSystem.h"

#include "World.h"

namespace GameEngine
{
    void CollisionSystem::enableCollisions(ColliderLayerId a, ColliderLayerId b, bool enabled)
    {
        permittedLayerCollisions[a][b] = enabled;
        permittedLayerCollisions[b][a] = enabled;
    }

    std::vector<Collision> CollisionSystem::detect(World& world)
    {
        std::vector<Collision> collisions;

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
                if (permittedLayerCollisions[layerAId][layerBId] == false)
                    continue;

                if (layerBId > layerAId)
                    break;

                for (size_t a = 0; a < colliderEntities[layerAId].size(); a++)
                {
                    for (size_t b = 0; b < colliderEntities[layerBId].size(); b++)
                    {
                        auto& entityA = colliderEntities[layerAId][a];
                        auto& entityB = colliderEntities[layerBId][b];

                        if (entityA == entityB)
                            continue;
                        if (b > a)
                            break;

                        if (entityA.right() < entityB.left()
                            || entityA.left() > entityB.right()
                            || entityA.bottom() > entityB.top()
                            || entityA.top() < entityB.bottom())
                        {
                            continue;
                        }

                        collisions.push_back(Collision{entityA.id(), entityB.id()});
                    }
                }
            }
        }

        return collisions;
    }

    void CollisionSystem::resolve(World& world, std::vector<Collision>& collisions)
    {
        for (auto& collision : collisions)
        {
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
