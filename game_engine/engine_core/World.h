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
#include "systems/HpCheckSystem.h"
#include "systems/ParticleSystem.h"

namespace EngineCore
{
    // Snapshot of game state written by the game each frame and consumed by the renderer.
    struct HudData
    {
        float playerHpCurrent = 100.0f;
        float playerHpMax     = 100.0f;
        int   wave            = 0;
        int   kills           = 0;
        float survivalTime    = 0.0f;
        bool  isDead          = false;
    };

    // Flat config struct used with the factory-style createEntity overload.
    // Designated initializers keep call sites readable with no positional guessing.
    struct EntitySpec
    {
        float x = 0, y = 0;
        float vx = 0, vy = 0;
        float width = 0.1f, height = 0.1f;
        GameColor color = white;
        State state = STATE_DEFAULT;
        ColliderShape colliderShape = ColliderShape::RECT;
        ColliderLayerId colliderLayerId = DEFAULT_COLLISION_LAYER_ID;
        AnimationData animationData = {};
        SpriteId spriteId = INVALID_SPRITE_ID;
        EntityTypeId entityType = 0;
        BehaviorId behaviorId = INVALID_BEHAVIOR_ID;
        float maxHp = 0.0f;
        float currentHp = 0.0f;
        float rotation = 0.0f;
        float particleLifetime = 0.0f;
        float particleMaxLifetime = 0.0f;
    };

    struct World
    {
        static constexpr uint32_t MAX_ENTITIES = 1'000'000;

        World(float frameDt);

        const float frameDt;

        GameColor backgroundColor = rayWhite;
        float viewportX = 0, viewportY = 0;
        float viewportScale = 2.0f; // pxPerGameUnit = minScreenDim / viewportScale

        HudData hud;

        CollisionSystem collisionSystem;
        AnimationSystem animationSystem;
        KinematicsSystem kinematicsSystem;
        HpCheckSystem hpCheckSystem;
        ParticleSystem particleSystem;

        std::vector<std::unique_ptr<Archetype>> archetypes;

        ArenaAllocator archetypeListAllocator{1 * 1024 * 1024};

        struct ArchetypeList
        {
            Archetype** archetypes;
            size_t count;
        };

        Archetype* createArchetype(ComponentMask mask);

        // positional overload — old call sites in EntityFactory still use this form
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
                              BehaviorId behaviorId,
                              float maxHp = 0.0f, float currentHp = 0.0f,
                              float rotation = 0.0f,
                              float particleLifetime = 0.0f, float particleMaxLifetime = 0.0f);

        // Factory-friendly overload using EntitySpec with designated initializers.
        EntityId createEntity(Archetype* archetype, const EntitySpec& spec);

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
                    archetypeList[matchCount++] = archetype.get();
            }

            return {archetypeList, matchCount};
        }

    private:
        std::vector<EntityRecord> entityRecords;
        std::vector<uint32_t> freeIndices;
    };
}

#endif //ENGINE_CORE_WORLD_H
