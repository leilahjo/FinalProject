//
// Created by greg8 on 2/15/2026.
//

#ifndef GAME_ENGINE_PLAYERSHIP_H
#define GAME_ENGINE_PLAYERSHIP_H

#include "engine_core/EngineCore.h"
#include "engine_runtime/Runtime.h"

namespace Game
{
    struct Game;

    using namespace EngineCore;

    struct PlayerShip
    {
        static constexpr float PLAYER_ACCEL = 60.0f / 6.0f;
        static constexpr float PLAYER_MAX_SPEED = 1.0f;
        static constexpr float LASER_SPEED = 4.0f;

        static constexpr uint64_t LASER_SHOOT_COOLDOWN_FRAMES = EngineRuntime::TARGET_FPS / 20; // 20 shots per second at N fps

        Archetype* playerArchetype;
        Archetype* laserArchetype;

        EntityId playerId;

        AnimationId shipRollLeftAnimation;
        AnimationId shipRollRightAnimation;
        AnimationId shipIdleAnimation;

        SpriteId playerShipSpriteSheetId;
        SpriteId playerLaserSpriteId;

        uint64_t nextShootFrame;

        void init(Game& game, World& world);
        void createLaserEntity(Game& game, World& world, Entity player, float horizontalOffset);
        void shootLaser(Game& game, World& world, Entity player);
        void update(Game& game, World& world);
        void enforceBoundary(Game& game, World& world);
    };
}


#endif //GAME_ENGINE_PLAYERSHIP_H
