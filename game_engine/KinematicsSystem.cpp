//
// Created by greg8 on 2/5/2026.
//

#include "KinematicsSystem.h"

namespace GameEngine
{
    void KinematicsSystem::Update(World& world, float frameDt)
    {
        for (ArchIndex archIndex = 0; archIndex < world.archetypes.size(); archIndex++)
        {
            auto& archetype = world.archetypes[archIndex];

            if (!archetype->hasVelocity() || !archetype->hasPosition())
                continue;

            for (EntityIndex entityIndex = 0; entityIndex < archetype->getEntityCount(); entityIndex++)
            {
                archetype->x[entityIndex] += archetype->vx[entityIndex] * frameDt;
                archetype->y[entityIndex] += archetype->vy[entityIndex] * frameDt;
            }
        }
    }
}
