//
// Created by greg8 on 2/5/2026.
//

#include "KinematicsSystem.h"

namespace GameEngine
{
    void KinematicsSystem::Update(World& world, float frameDt)
    {
        world.forEach(Archetype::COMP_POSITION | Archetype::COMP_VELOCITY,
        [frameDt](Entity entity){
            entity.x() += entity.vx() * frameDt;
            entity.y() += entity.vy() * frameDt;
        });
    }
}
