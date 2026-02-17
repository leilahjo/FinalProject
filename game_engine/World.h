//
// Created by greg8 on 2/5/2026.
//

#ifndef GAME_ENGINE_WORLD_H
#define GAME_ENGINE_WORLD_H
#include <memory>
#include <optional>
#include <vector>

#include "Archetype.h"
#include "ECSTypes.h"
#include "Entity.h"

namespace GameEngine
{
    struct World
    {
        static constexpr uint32_t MAX_ENTITIES = 1'000'000;

        World();

        // We're using unique_ptr because world is the exclusive "owner" of the archetypes.
        std::vector<std::unique_ptr<Archetype>> archetypes;

        Archetype* createArchetype(ComponentMask mask);
        EntityId createEntity(Archetype* archetype,
                              float x, float y,
                              float vx, float vy,
                              float width, float height,
                              Color color,
                              Entity::State state,
                              ColliderShape colliderShape, ColliderLayerId colliderLayerId);

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

    private:
        std::vector<EntityRecord> entityRecords;
        std::vector<uint32_t> freeIndices;
    };
}


#endif //GAME_ENGINE_WORLD_H
