//
// Created by greg8 on 2/5/2026.
//

#ifndef GAME_ENGINE_WORLD_H
#define GAME_ENGINE_WORLD_H
#include <memory>
#include <vector>

#include "Entity.h"
#include "Archetype.h"

namespace GameEngine
{
    struct World
    {
        static constexpr int MAX_ENTITIES = 1'000'000;

        World();

        // We're using unique_ptr because world is the exclusive "owner" of the archetypes.
        std::vector<std::unique_ptr<Archetype>> archetypes;

        Archetype* createArchetype(ComponentMask mask)
        {
            archetypes.push_back(std::make_unique<Archetype>(mask));
            return archetypes.back().get();
        }

        Entity createEntity(Archetype* archetype,
                            float x, float y,
                            float vx, float vy,
                            float width, float height,
                            Color color,
                            Entity::State state);

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
