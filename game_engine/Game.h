//
// Created by greg8 on 1/29/2026.
//

#ifndef GAME_ENGINE_GAME_H
#define GAME_ENGINE_GAME_H
#include <vector>

#include "GameObject.h"
#include "InputManager.h"
#include "KinematicsData.h"

using namespace GameEngine;

namespace Game
{
    struct Game
    {
        static constexpr float frameDt = 1 / 60.0f;

        Game();

        GameObject* player;
        std::vector<GameObject> objects;
        KinematicsData kinematicsData;

        void Update(InputManager& inputManager);
    };
}

#endif //GAME_ENGINE_GAME_H