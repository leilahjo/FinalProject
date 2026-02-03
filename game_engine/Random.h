//
// Created by greg8 on 2/3/2026.
//

#ifndef GAME_ENGINE_RANDOM_H
#define GAME_ENGINE_RANDOM_H
#include <random>

namespace GameEngine
{
    static std::mt19937 rng{std::random_device{}()};
    static std::uniform_real_distribution<float> dist(-1, 1);

    float randomFloat(float min, float max)
    {
        float v = dist(rng);
        return min + (max - min) * (v * 0.5f + 0.5f);
    }
}

#endif //GAME_ENGINE_RANDOM_H