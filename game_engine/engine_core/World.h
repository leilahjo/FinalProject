//
// Created by greg8 on 2/5/2026.
//

#ifndef ENGINE_CORE_WORLD_H
#define ENGINE_CORE_WORLD_H

#include <memory>
#include <optional>
#include <vector>

#include "ecs/Archetype.h"
#include "ecs/ECSTypes.h"
#include "ecs/Entity.h"
#include "systems/AnimationSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/KinematicsSystem.h"

namespace EngineCore
{
    struct World
    {
        static constexpr uint32_t MAX_ENTITIES = 1'000'000;

        World(float frameDt);

        const float frameDt;

        GameColor backgroundColor = rayWhite;
		float viewportX = 0, viewportY = 0;

        CollisionSystem collisionSystem;
        AnimationSystem animationSystem;
        KinematicsSystem kinematicsSystem;

        // We're using unique_ptr because world is the exclusive "owner" of the archetypes.
        std::vector<std::unique_ptr<Archetype>> archetypes;

        // 1MB allocator specifically for archetype queries
        ArenaAllocator archetypeListAllocator{1 * 1024 * 1024};

        // A lightweight view over our arena-allocated array
        struct ArchetypeList {
            Archetype** archetypes;
            size_t count;
        };

        Archetype* createArchetype(ComponentMask mask);
        EntityId createEntity(Archetype* archetype,
                              float x, float y,
                              float vx, float vy,
                              float width, float height,
                              GameColor color,
                              State state,
                              ColliderShape colliderShape, ColliderLayerId colliderLayerId,
                              AnimationData animationData,
                              SpriteId spriteId,
                              EntityTypeId entityType,
                              BehaviorId behaviorId);

        bool removeEntity(EntityId entityId);

        std::optional<Entity> findEntity(EntityId entityId);

        void cleanup();

        template <typename Callable>
        void forEach(ComponentMask mask, Callable&& callable)
        {
            for (auto& archetype : archetypes)
            {
                if ((archetype->componentMask & mask) != mask)
                    continue;

                for (EntityIndex entityIndex = 0; entityIndex < archetype->getEntityCount(); ++entityIndex)
                    callable(Entity{archetype.get(), entityIndex});
            }
        }

        ArchetypeList getMatchingArchetypes(ComponentMask mask)
        {
            archetypeListAllocator.resetFully();
            auto** archetypeList = archetypeListAllocator.allocate<Archetype*>(archetypes.size());

            size_t matchCount = 0;
            for (const auto& archetype : archetypes)
            {
                if ((archetype->componentMask & mask) == mask)
                {
                    archetypeList[matchCount] = archetype.get();
                    matchCount++;
                }
            }

            return {archetypeList, matchCount};
        }

    private:
        std::vector<EntityRecord> entityRecords;
        std::vector<uint32_t> freeIndices;
    };
}


#endif //ENGINE_CORE_WORLD_H
