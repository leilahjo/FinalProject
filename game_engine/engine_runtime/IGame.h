//
// Created by greg8 on 2/21/2026.
//

#ifndef ENGINE_RUNTIME_ENGINEINTERFACES_H
#define ENGINE_RUNTIME_ENGINEINTERFACES_H

#include "engine_core/EngineCore.h"

namespace EngineRuntime
{
    using namespace EngineCore;

    struct IGame
    {
        virtual void onStart(World& world) = 0;
        virtual void onUpdateBegin(World& world) = 0;
        virtual void onUpdatePostKinematics(World& world) = 0;
        virtual void onUpdatePostCollisionDetection(World& world, Collision collisions[], size_t collisionCount) = 0;
        virtual void onUpdatePostCollisionResolution(World& world, Collision collisions[], size_t collisionCount) = 0;

        virtual ~IGame() = default;
    };
}

#endif //ENGINE_RUNTIME_ENGINEINTERFACES_H
