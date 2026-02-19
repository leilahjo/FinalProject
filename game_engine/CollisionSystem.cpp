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
}
