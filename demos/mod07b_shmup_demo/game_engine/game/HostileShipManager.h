//
// Created by greg8 on 2/15/2026.
//

#ifndef GAME_ENGINE_HOSTILESHIPMANAGER_H
#define GAME_ENGINE_HOSTILESHIPMANAGER_H

#include "engine_core/EngineCore.h"

namespace Game
{
    struct Game;
    using namespace EngineCore;

    enum struct SpawnMode
    {
        NONE,
        WAVES,
        RANDOM
    };

    struct HostileShipManager
    {
        static constexpr float HOSTILE_SHIP_SPEED = 0.5f;
        static constexpr float HOSTILE_SHIP_WIDTH = 0.1f;
        static constexpr float HOSTILE_SHIP_HEIGHT = HOSTILE_SHIP_WIDTH * 128 / 56;

        Archetype* hostileShipArchetype;

        SpriteId hostileShipSpriteId;

        void init(Game& game, World& world);

        void update(Game& game, World& world);
        void updateSpawnsWaves(Game& game, World& world);
        void updateSpawnsRandom(Game& game, World& world);
        void spawnHostileShip(Game& game, World& world, float x);
        void enforceBoundary(Game& game, World& world);

        SpawnMode spawnMode = SpawnMode::NONE;
    };
}


#endif //GAME_ENGINE_HOSTILESHIPMANAGER_H
