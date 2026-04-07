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
        Archetype* faunaArchetype;

        SpriteId playerRunSpriteSheetId;
        SpriteId rockSpriteId;
        SpriteId lemmingSpriteId;
        SpriteId pigSpriteSheetId;
        SpriteId chickenSpriteSheetId;
        SpriteId sheepSpriteSheetId;

        BehaviorId lemmingBehaviorId;
        BehaviorId faunaBehaviorId;

        AnimationId simpleAnimationId;
        AnimationId playerRunAnimationId;
        AnimationId animalIdleAnimation;

        SoundId dootSoundId;

        void onStart(World& world) override;
        void onUpdateBegin(World& world) override;
        void onUpdatePostKinematics(World& world) override;
        void onUpdatePostCollisionDetection(World& world, Collision collisions[], size_t collisionCount) override;
        void onUpdatePostCollisionResolution(World& world, Collision collisions[], size_t collisionCount) override;

        int run();

        enum EntityType : EntityTypeId
        {
            ENTITY_TYPE_NONE,
            ENTITY_TYPE_PLAYER,
            ENTITY_TYPE_ROCK,
            ENTITY_TYPE_ANIMATED_SQUARE,
            ENTITY_TYPE_FAUNA
        };

        enum Layers : ColliderLayerId
        {
            LAYER_NONE,
            LAYER_PLAYER,
            LAYER_ROCKS,
            LAYER_ANIMATED_SQUARES,
            LAYER_FAUNA,
			LAYER_LEMMINGS,
        };
    };
}

#endif //GAME_ENGINE_GAME_H
