#pragma once

#include "engine_core/EngineCore.h"
#include "EntityFactory.h"

namespace Game
{
    using namespace EngineCore;

    // Tracks waves, spawns enemies at arena edges, and prints wave-start logs.
    struct WaveManager
    {
        int   wave         = 0;
        int   enemiesLeft  = 0;   // alive enemy count this wave
        float graceTimer   = 2.0f; // countdown before first wave

        // Call each frame. Returns true when a new wave just started.
        bool update(float dt, World& world, EntityFactory& factory);

        void notifyEnemyKilled() { if (enemiesLeft > 0) enemiesLeft--; }

    private:
        void spawnWave(World& world, EntityFactory& factory);
        static void randomEdgePos(float& x, float& y);
    };
}
