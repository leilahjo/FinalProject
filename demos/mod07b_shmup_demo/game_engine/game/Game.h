//
// Created by greg8 on 1/29/2026.
//

#ifndef GAME_ENGINE_GAME_H
#define GAME_ENGINE_GAME_H

#include "HostileShipManager.h"
#include "PlayerShip.h"
#include "StarField.h"
#include "util/Rect.h"

#include "engine_runtime/EngineRuntime.h"


namespace Game
{
    using namespace EngineRuntime;

    struct Game final : IGame
    {
        Runtime runtime;

        Archetype* particleArchetype;
        AnimationId particleAlphaFadeAnimationId;

        StarField starField;
        PlayerShip playerShip;
        HostileShipManager hostileShipManager;

        Rect playerBoundary = {-1, 1, -1 * 800.0f / 600, 1 * 800.0f / 600};
        Rect playerProjectileBoundary = {-1, 1, -2, 2};
        Rect hostileShipBoundary = {-1, 1, -2, 2};
        Rect starBoundary = {-1, 1, -2, 2};

        int run();

        void onStart(World& world) override;
        void onUpdateBegin(World& world) override;
        void onUpdatePostKinematics(World& world) override;
        void onUpdatePostCollisionDetection(World& world, std::vector<Collision>& collisions) override;
        void onUpdatePostCollisionResolution(World& world, std::vector<Collision>& vector) override;

        void explode(Entity entity, World& world);
        void updateParticles();

        enum Layers : ColliderLayerId
        {
            LAYER_NONE,
            LAYER_PLAYER,
            LAYER_LASERS,
            LAYER_HOSTILE_SHIP_PROJECTILES,
            LAYER_HOSTILE_SHIPS
        };
    };
}

#endif //GAME_ENGINE_GAME_H
