//
// Created by Gregory Anderson on 2/9/26.
//

#include "Random.h"
#include <random>

namespace EngineCore
{
    static uint32_t globalSeed = std::random_device{}();

    void setSeed(uint32_t seed)
    {
        globalSeed = seed;
    }

    float randomFloat(float min, float max)
    {
        thread_local std::mt19937 rng{globalSeed};
        std::uniform_real_distribution dist(min, max);
        return dist(rng);
    }
}
