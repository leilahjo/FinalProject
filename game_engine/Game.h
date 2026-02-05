//
// Created by greg8 on 1/29/2026.
//

#ifndef GAME_ENGINE_GAME_H
#define GAME_ENGINE_GAME_H
#include <vector>

#include "Entity.h"
#include "InputManager.h"
#include "Archetype.h"
#include "World.h"

using namespace GameEngine;

namespace Game
{
    struct Game
    {
        static constexpr float frameDt = 1 / 60.0f;

        Game();

        Entity player;
        World world;
        std::vector<Entity> objects;

        //No velocity.
        Archetype* borderArchetype;

        // Velocity.
        Archetype* squareArchetype;

        void Update(InputManager& inputManager);
    };
}

#endif //GAME_ENGINE_GAME_H