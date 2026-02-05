//
// Created by greg8 on 2/5/2026.
//

#ifndef GAME_ENGINE_WORLD_H
#define GAME_ENGINE_WORLD_H
#include <memory>
#include <vector>

#include "Entity.h"

namespace GameEngine
{
    struct World
    {
        // We're using unique_ptr because world is the exclusive "owner" of the archetypes.
        std::vector<std::unique_ptr<Archetype>> archetypes;

        Archetype* createArchetype(Archetype::ComponentMask mask)
        {
            archetypes.push_back(std::make_unique<Archetype>(mask));
            return archetypes.back().get();
        }

        Entity createEntity(Archetype* archetype, float x, float y, float vx, float vy,
                            float width, float height, Color color)
        {
            ArchIndex index = archetype->create(x, y, vx, vy, width, height, color);
            return Entity(archetype, index);
        }
    };
}


#endif //GAME_ENGINE_WORLD_H
