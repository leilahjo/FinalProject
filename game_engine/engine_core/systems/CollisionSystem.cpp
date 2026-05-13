//
// Created by greg8 on 2/17/2026.
//

#include <vector>
#include <cmath>

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

        for (auto& v : colliderEntities) v.clear();
        world.forEach(Archetype::COMP_POSITION | Archetype::COMP_SIZE | Archetype::COMP_COLLIDER,
                      [this](Entity entity)
                      {
                          this->colliderEntities[entity.colliderLayerId()].push_back(entity);
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

                        // Shape-aware overlap test.
                        // Note: no spatial broadphase, so cache locality is limited by design.
                        bool aCircle = entityA.colliderShape() == ColliderShape::CIRCLE;
                        bool bCircle = entityB.colliderShape() == ColliderShape::CIRCLE;
                        bool overlapping;

                        if (aCircle && bCircle)
                        {
                            float ra = std::min(entityA.width(), entityA.height()) * 0.5f;
                            float rb = std::min(entityB.width(), entityB.height()) * 0.5f;
                            float dx = entityA.x() - entityB.x();
                            float dy = entityA.y() - entityB.y();
                            overlapping = (dx * dx + dy * dy) < (ra + rb) * (ra + rb);
                        }
                        else
                        {
                            overlapping = !(entityA.right() < entityB.left() ||
                                            entityA.left()  > entityB.right() ||
                                            entityA.top()   < entityB.bottom() ||
                                            entityA.bottom()> entityB.top());
                        }

                        if (!overlapping)
                            continue;

                        auto newCollisionPtr = allocator.allocate<Collision>(1);
                        if (newCollisionPtr == nullptr)
                            return collisions;
                        *newCollisionPtr = Collision{entityA.id(), entityB.id()};
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
            if (a.state() & STATE_DESTROYED || b.state() & STATE_DESTROYED)
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
        if (!aHasVelocity) { moveA = 0;                    moveB = 1 + separationEpsilon; }
        if (!bHasVelocity) { moveA = 1 + separationEpsilon; moveB = 0; }

        bool aCircle = a.entityLocation.archetype->hasCollider() &&
                       a.colliderShape() == ColliderShape::CIRCLE;
        bool bCircle = b.entityLocation.archetype->hasCollider() &&
                       b.colliderShape() == ColliderShape::CIRCLE;

        if (aCircle && bCircle)
        {
            // Push apart along the line connecting centers.
            float ra = std::min(a.width(), a.height()) * 0.5f;
            float rb = std::min(b.width(), b.height()) * 0.5f;
            float dx = a.x() - b.x();
            float dy = a.y() - b.y();
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < 0.0001f) { dx = 1.0f; dy = 0.0f; dist = 1.0f; }
            float overlap = ra + rb - dist;
            float nx = dx / dist, ny = dy / dist;

            a.x() += nx * overlap * moveA;
            a.y() += ny * overlap * moveA;
            b.x() -= nx * overlap * moveB;
            b.y() -= ny * overlap * moveB;

            // Cancel velocity components pointing into the collision.
            if (aHasVelocity)
            {
                float dot = a.vx() * nx + a.vy() * ny;
                if (dot < 0) { a.vx() -= 2 * dot * nx; a.vy() -= 2 * dot * ny; }
            }
            if (bHasVelocity)
            {
                float dot = b.vx() * nx + b.vy() * ny;
                if (dot > 0) { b.vx() -= 2 * dot * nx; b.vy() -= 2 * dot * ny; }
            }
            return;
        }

        // AABB separation for rect-rect and rect-circle combos.
        float xOverlap = std::min(a.right(), b.right()) - std::max(a.left(), b.left());
        float yOverlap = std::min(a.top(), b.top()) - std::max(a.bottom(), b.bottom());

        bool resolveX = xOverlap < yOverlap;
        float& aPosition = resolveX ? a.x() : a.y();
        float& aVelocity = resolveX ? a.vx() : a.vy();
        float& bPosition = resolveX ? b.x() : b.y();
        float& bVelocity = resolveX ? b.vx() : b.vy();
        float& overlap   = resolveX ? xOverlap : yOverlap;

        if (aPosition < bPosition)
        {
            aPosition -= overlap * moveA;
            bPosition += overlap * moveB;
            if (aVelocity > 0) aVelocity *= -1;
            if (bVelocity < 0) bVelocity *= -1;
        }
        else
        {
            aPosition += overlap * moveA;
            bPosition -= overlap * moveB;
            if (aVelocity < 0) aVelocity *= -1;
            if (bVelocity > 0) bVelocity *= -1;
        }
    }
}
