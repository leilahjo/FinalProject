//
// Created by greg8 on 3/7/2026.
//

#include "BehaviorManager.h"

#include "EntityProvider.h"
#include <iostream>

namespace Scripting
{
    void BehaviorManager::initialize(uint16_t maxBehaviors)
    {
        behaviors.resize(maxBehaviors);
        freeIndices.resize(maxBehaviors);
        for (size_t i = 0; i < freeIndices.size(); i++)
            freeIndices[i] = maxBehaviors - i - 1;

        lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);

        // Bind the inner EntityId struct
        lua.new_usertype<EntityId>("EntityId",
                                   "recordIndex", &EntityId::recordIndex,
                                   "generation", &EntityId::generation
        );

        // Bind the Entity proxy so Lua can modify the data we care about.
        lua.new_usertype<Entity>("Entity",
                                 "x", sol::property(
                                     [](Entity& entity) -> float { return entity.x(); },
                                     [](Entity& entity, float value) { entity.x() = value; }
                                 ),
                                 "y", sol::property(
                                     [](Entity& entity) -> float { return entity.y(); },
                                     [](Entity& entity, float value) { entity.y() = value; }
                                 ),
                                 "vx", sol::property(
                                     [](Entity& entity) -> float { return entity.vx(); },
                                     [](Entity& entity, float value) { entity.vx() = value; }
                                 ),
                                 "vy", sol::property(
                                     [](Entity& entity) -> float { return entity.vy(); },
                                     [](Entity& entity, float value) { entity.vy() = value; }
                                 )
        );

        // Bind the EntityProvider's getEntity function.
        lua.new_usertype<EntityProvider>("EntityProvider", "getEntity",
                                         &EntityProvider::getEntity);

        lua.set_function("applyFleeBehavior",
                         [](std::vector<Entity>& entities, float px, float py, float fleeRadius, float speed)
                         {
                             float radiusSq = fleeRadius * fleeRadius;
                             for (auto& entity : entities)
                             {
                                 float dx = px - entity.x();
                                 float dy = py - entity.y();
                                 float distSq = (dx * dx) + (dy * dy);

                                 if (distSq < radiusSq)
                                 {
                                     float dist = std::sqrt(distSq);
                                     entity.vx() = -(dx / dist) * speed;
                                     entity.vy() = -(dy / dist) * speed;
                                 }
                                 else if (entity.vx() != 0 || entity.vy() != 0)
                                 {
                                     entity.vx() = 0;
                                     entity.vy() = 0;
                                 }
                             }
                         });
    }

    void BehaviorManager::uninitialize()
    {
        for (Behavior& behavior : behaviors)
            behavior.unload();
        behaviors.clear();
        freeIndices.clear();
    }

    BehaviorId BehaviorManager::loadBehavior(const std::string& scriptPath)
    {
        if (freeIndices.empty())
            return INVALID_BEHAVIOR_ID;

        uint16_t index = freeIndices.back();
        auto& behavior = behaviors[index];
        if (!behavior.load(lua, scriptPath))
            return INVALID_BEHAVIOR_ID;

        freeIndices.pop_back();
        return {index, behavior.generation};
    }

    void BehaviorManager::reloadAllBehaviors()
    {
        for (auto& behavior : behaviors)
        {
            if (behavior.path.empty())
                continue;

            behavior.reload(lua);
        }
    }

    bool BehaviorManager::unloadBehavior(BehaviorId id)
    {
        if (id.index >= behaviors.size())
            return false;
        auto& behavior = behaviors[id.index];
        if (behavior.generation != id.generation)
            return false;
        behavior.unload();
        freeIndices.push_back(id.index);
        return true;
    }

    void BehaviorManager::setGlobalEntityId(const std::string& variableName, EntityId id)
    {
        lua[variableName] = id;
    }

    void BehaviorManager::update(World& world, BlockTimer& blockTimer)
    {
        blockTimer.startBlock("behavior startup");
        groups.clear();

        blockTimer.startBlock("behavior grouping");
        // Step 1. Group entities by behavior and populate Entities for them.
        world.forEach(Archetype::COMP_BEHAVIOR, [&](Entity entity)
        {
            auto behaviorId = entity.behaviorId();
            if (behaviorId == INVALID_BEHAVIOR_ID)
                return;
            if (behaviorId.index >= behaviors.size() || behaviors[behaviorId.index].generation != behaviorId.generation)
                return;
            
            groups[behaviorId.index].entities.push_back(entity);
        });

        EntityProvider entityProvider{world};

        blockTimer.startBlock("behavior execution");
        BlockTimer behaviorExecutionTimer = BlockTimer("Behavior Execution");
        // Step 2. Execute behaviors for each group
        for (auto& [index, group] : groups)
        {
            behaviorExecutionTimer.startBlock("check validity");
            auto& behavior = behaviors[index];
            if (!behavior.updateFunction.valid())
            {
                behaviorExecutionTimer.endFrame();
                continue;
            }

            behaviorExecutionTimer.startBlock("update");
            // std::ref forces sol to pass by reference.
            auto result = behavior.updateFunction(std::ref(group.entities), std::ref(entityProvider));
            behaviorExecutionTimer.startBlock("check result");
            if (!result.valid())
            {
                sol::error err = result;
                std::cerr << err.what() << std::endl;
            }
            behaviorExecutionTimer.endFrame();
        }
        // behaviorExecutionTimer.printStats();
    }
}
