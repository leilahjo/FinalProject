//
// Created by greg8 on 2/5/2026.
//

#ifndef ENGINE_CORE_KINEMATICSSYSTEM_H
#define ENGINE_CORE_KINEMATICSSYSTEM_H

namespace EngineCore
{
    struct World;

    struct KinematicsSystem
    {
        void update(World& world);
    };
}

#endif //ENGINE_CORE_KINEMATICSSYSTEM_H
