//
// Created by greg8 on 1/29/2026.
//

#ifndef GAME_ENGINE_GAME_H
#define GAME_ENGINE_GAME_H

#include "AnimationSystem.h"
#include "InputManager.h"
#include "Archetype.h"
#include "CollisionSystem.h"
#include "SpriteManager.h"
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

        Archetype* borderArchetype;
        Archetype* playerArchetype;
        Archetype* rockArchetype;
        Archetype* animatedSquareArchetype;

        AnimationId simpleAnimationId;
        AnimationId playerAnimationId;

        SpriteId rockSpriteId;
        SpriteId playerSpriteSheetId;

        CollisionSystem collisionSystem;
        AnimationSystem animationSystem;
        SpriteManager spriteManager;

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
