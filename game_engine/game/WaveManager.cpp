#include "WaveManager.h"

#include <iostream>
#include "engine_core/util/Random.h"

namespace Game
{
    static constexpr float ARENA_W = 3.8f; // spawn just outside arena edge
    static constexpr float ARENA_H = 2.8f;

    void WaveManager::randomEdgePos(float& x, float& y)
    {
        int edge = static_cast<int>(randomFloat(0.0f, 4.0f)); // 0-3
        switch (edge)
        {
        case 0: x = randomFloat(-ARENA_W, ARENA_W); y =  ARENA_H; break; // top
        case 1: x = randomFloat(-ARENA_W, ARENA_W); y = -ARENA_H; break; // bottom
        case 2: x =  ARENA_W; y = randomFloat(-ARENA_H, ARENA_H); break; // right
        default:x = -ARENA_W; y = randomFloat(-ARENA_H, ARENA_H); break; // left
        }
    }

    bool WaveManager::update(float dt, World& world, EntityFactory& factory)
    {
        if (enemiesLeft > 0)
            return false;

        graceTimer -= dt;
        if (graceTimer > 0.0f)
            return false;

        wave++;
        spawnWave(world, factory);
        graceTimer = 4.0f; // time before checking again after wave clears
        return true;
    }

    void WaveManager::spawnWave(World& world, EntityFactory& factory)
    {
        int chasers = 3 + wave * 2;
        int tanks   = (wave >= 2) ? wave - 1 : 0;
        enemiesLeft = chasers + tanks;

        std::cout << "[Wave " << wave << "] spawning " << chasers
                  << " chasers + " << tanks << " tanks\n";

        float x, y;
        for (int i = 0; i < chasers; i++)
        {
            randomEdgePos(x, y);
            factory.spawnChaser(world, x, y);
        }
        for (int i = 0; i < tanks; i++)
        {
            randomEdgePos(x, y);
            factory.spawnTank(world, x, y);
        }
    }
}
