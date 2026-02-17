//
// Created by greg8 on 1/29/2026.
//

#ifndef GAME_ENGINE_GAME_H
#define GAME_ENGINE_GAME_H

#include "InputManager.h"
#include "Archetype.h"
#include "CollisionSystem.h"
#include "World.h"

using namespace GameEngine;

namespace Game
{
    struct Game
    {
        static constexpr float frameDt = 1 / 60.0f;

        Game();

        EntityId playerId;
        World world;

        //No velocity.
        Archetype* borderArchetype;

        // Velocity.
        Archetype* squareArchetype;

        CollisionSystem collisionSystem;

        void Update(InputManager& inputManager);

        enum Layers : ColliderLayerId
        {
            LAYER_NONE,
            LAYER_PLAYER,
            LAYER_RED_SQUARES,
            LAYER_BLUE_SQUARES
        };
    };
}

#endif //GAME_ENGINE_GAME_H
