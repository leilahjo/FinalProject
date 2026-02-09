//
// Created by Gregory Anderson on 2/9/26.
//

#include "Random.h"
#include <random>

float GameEngine::randomFloat(float min, float max) {
    thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}
