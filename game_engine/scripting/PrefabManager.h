//
// Created by greg8 on 3/10/2026.
//

#ifndef GAME_ENGINE_PREFABREGISTRY_H
#define GAME_ENGINE_PREFABREGISTRY_H
#include <map>

#include "Prefab.h"

namespace Scripting
{
    struct PrefabManager
    {
        void registerPrefab(const std::string& name, Prefab prefab);
        EntityId spawnPrefab(const std::string& name, World& world, float x, float y);

    private:
        std::map<std::string, Prefab> prefabs;
    };
}


#endif //GAME_ENGINE_PREFABREGISTRY_H