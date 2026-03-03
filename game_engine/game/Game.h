//
// Created by greg8 on 1/29/2026.
//

#ifndef GAME_ENGINE_GAME_H
#define GAME_ENGINE_GAME_H

#include "../engine_runtime/EngineRuntime.h"

namespace Game
{
    using namespace EngineRuntime;

    struct Game final : IGame
    {
        Runtime runtime;

        EntityId playerId;

        Archetype* borderArchetype;
        Archetype* rockArchetype;
        Archetype* animatedSquareArchetype;
        Archetype* playerArchetype;

        SpriteId playerRunSpriteSheetId;
        SpriteId rockSpriteId;

        AnimationId simpleAnimationId;
        AnimationId playerRunAnimationId;

        void onStart(World& world) override;
        void onUpdateBegin(World& world) override;
        void onUpdatePostKinematics(World& world) override;
        void onUpdatePostCollisionDetection(World& world, std::vector<Collision>& collisions) override;
        void onUpdatePostCollisionResolution(World& world, std::vector<Collision>& vector) override;
        int run();

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
