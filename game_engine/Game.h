//
// Created by greg8 on 1/29/2026.
//

#ifndef GAME_ENGINE_GAME_H
#define GAME_ENGINE_GAME_H
#include <vector>

#include "GameObject.h"
#include "InputManager.h"

namespace Game
{
    using namespace GameEngine;

    struct Game
    {
        static constexpr float frameDt = 1 / 60.0f;

        GameObject player;
        std::vector<GameObject> objects;

        void Update();
        void Update(InputManager& inputManager);
    };
}

#endif //GAME_ENGINE_GAME_H