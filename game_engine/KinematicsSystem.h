//
// Created by greg8 on 2/5/2026.
//

#ifndef GAME_ENGINE_KINEMATICSSYSTEM_H
#define GAME_ENGINE_KINEMATICSSYSTEM_H
#include "World.h"

namespace GameEngine
{
    struct KinematicsSystem
    {
        static void Update(World& world, float frameDt);
    };
}

#endif //GAME_ENGINE_KINEMATICSSYSTEM_H
