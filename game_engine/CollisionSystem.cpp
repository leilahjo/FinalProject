//
// Created by greg8 on 2/17/2026.
//

#include "CollisionSystem.h"

#include "World.h"

void GameEngine::CollisionSystem::enableCollisions(ColliderLayerId a, ColliderLayerId b, bool enabled)
{
    permittedLayerCollisions[a][b] = enabled;
    permittedLayerCollisions[b][a] = enabled;
}

std::vector<GameEngine::Collision> GameEngine::CollisionSystem::detectCollisions(World& world)
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

            for (Entity entityA : colliderEntities[layerAId])
            {
                for (Entity entityB : colliderEntities[layerBId])
                {
                    if (entityA == entityB)
                        continue;

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
