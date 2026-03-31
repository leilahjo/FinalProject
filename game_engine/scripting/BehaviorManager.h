//
// Created by greg8 on 3/7/2026.
//

#ifndef GAME_ENGINE_BEHAVIORMANAGER_H
#define GAME_ENGINE_BEHAVIORMANAGER_H
#include <string>
#include <vector>

#include <sol/sol.hpp>

#include "Behavior.h"
#include "../engine_core/EngineCore.h"

namespace Scripting
{
    using namespace EngineCore;

    struct BehaviorManager
    {
        void initialize(uint16_t maxBehaviors = 1024);
        void uninitialize();

        BehaviorId loadBehavior(const std::string& scriptPath);
        void reloadAllBehaviors();
        bool unloadBehavior(BehaviorId id);

        void setGlobalEntityId(const std::string& variableName, EntityId id);

        void update(World& world);

    private:
        sol::state lua;

        std::vector<Behavior> behaviors;
        std::vector<uint16_t> freeIndices;

        struct BehaviorGroup
        {
            std::vector<Entity> entities;
        };

        std::unordered_map<uint16_t, BehaviorGroup> groups;
    };
}

#endif //GAME_ENGINE_BEHAVIORMANAGER_H
