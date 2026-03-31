//
// Created by greg8 on 3/7/2026.
//

#ifndef GAME_ENGINE_ENTITYPROVIDER_H
#define GAME_ENGINE_ENTITYPROVIDER_H

#include "engine_core/EngineCore.h"

namespace Scripting
{
    using namespace EngineCore;

    struct EntityProvider
    {
        World& world;

        EntityProvider(World& world) : world(world)
        {
        }

        Entity getEntity(EntityId id)
        {
            auto optEntity = world.findEntity(id);
            if (!optEntity)
                return {};
            return optEntity.value();
        }
    };
}

#endif //GAME_ENGINE_ENTITYPROVIDER_H
