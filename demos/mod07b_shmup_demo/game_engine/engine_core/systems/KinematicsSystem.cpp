//
// Created by greg8 on 2/5/2026.
//

#include "KinematicsSystem.h"
#include "../World.h"
#include "../ecs/Archetype.h"

namespace EngineCore
{
    void KinematicsSystem::update(World& world)
    {
        world.forEach(Archetype::COMP_POSITION | Archetype::COMP_VELOCITY,
                      [&](Entity entity)
                      {
                          entity.x() += entity.vx() * world.frameDt;
                          entity.y() += entity.vy() * world.frameDt;
                      });
    }
}
