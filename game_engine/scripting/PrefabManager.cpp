//
// Created by greg8 on 3/10/2026.
//

#include "PrefabManager.h"

namespace Scripting
{
    void PrefabManager::registerPrefab(const std::string& name, Prefab prefab)
    {
        prefabs[name] = prefab;
    }

    EntityId PrefabManager::spawnPrefab(const std::string& name, World& world, float x, float y)
    {
        auto prefbEntry = prefabs.find(name);
        if (prefbEntry == prefabs.end())
            return INVALID_ENTITY_ID;
        auto& prefab = prefbEntry->second;

        auto animationProgress = prefab.animationData.progress;
        if (animationProgress < 0.0f || animationProgress > 1.0f)
            animationProgress = randomFloat(0, 1);
        auto animationSpeed = prefab.animationData.speed;
        if (animationSpeed < 0.0f)
            animationSpeed = randomFloat(0.9, 1.1);

        return world.createEntity(prefab.archetype,
                                  x, y,
                                  0, 0,
                                  prefab.width, prefab.height,
                                  prefab.color,
                                  prefab.state,
                                  prefab.colliderShape, prefab.colliderLayerId,
                                  AnimationData{
                                      prefab.animationData.animationId,
                                      animationProgress,
                                      animationSpeed,
                                      prefab.animationData.state
                                  },
                                  prefab.spriteId,
                                  prefab.entityTypeId,
                                  prefab.behaviorId);
    }
}
