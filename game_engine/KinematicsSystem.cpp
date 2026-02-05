//
// Created by greg8 on 2/5/2026.
//

#include "KinematicsSystem.h"

namespace GameEngine
{
    void KinematicsSystem::Update(World& world, float frameDt)
    {
        for (ArchIndex i = 0; i < world.archetypes.size(); i++)
        {
            auto archetype = world.archetypes[i].get();

            if (!archetype->hasVelocity() || !archetype->hasPosition())
                continue;

            for (int i = 0; i < archetype->getCount(); i++)
            {
                archetype->x[i] += archetype->vx[i] * frameDt;
                archetype->y[i] += archetype->vy[i] * frameDt;
            }
        }
    }
}
